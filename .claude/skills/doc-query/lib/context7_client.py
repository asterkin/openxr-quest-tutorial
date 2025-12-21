"""Context7 API client for documentation queries.

Uses only Python 3.14+ stdlib (urllib, tomllib, pathlib).
"""

import json
import os
import tomllib
import urllib.parse
import urllib.request
import urllib.error
from pathlib import Path


class Context7Error(Exception):
    """Base exception for Context7 API errors."""


class Context7AuthError(Context7Error):
    """Authentication failed (missing or invalid API key)."""


class Context7NotFoundError(Context7Error):
    """Requested library not found."""


class Context7RateLimitError(Context7Error):
    """Rate limit exceeded."""

    def __init__(self, message: str, retry_after: int | None = None):
        super().__init__(message)
        self.retry_after = retry_after


API_BASE = "https://context7.com/api/v1"
DEFAULT_TOKENS = 2500
TIMEOUT_SECONDS = 30


def _find_project_root() -> Path:
    """Find project root by looking for .claude directory."""
    current = Path.cwd()
    for parent in [current, *current.parents]:
        if (parent / ".claude").is_dir():
            return parent
    raise Context7Error("Cannot find project root (.claude directory)")


def load_doc_sources() -> dict:
    """Load documentation sources from .claude/doc-sources.toml."""
    config_path = _find_project_root() / ".claude" / "doc-sources.toml"
    if not config_path.exists():
        raise FileNotFoundError(
            f"Configuration not found: {config_path}\n"
            "Run add-doc skill to configure documentation sources."
        )
    with open(config_path, "rb") as f:
        data = tomllib.load(f)
    return data.get("sources", {})


def resolve_source(name: str) -> tuple[str, int]:
    """Resolve source name/alias to Context7 ID and default tokens."""
    sources = load_doc_sources()

    # Direct match
    if name in sources:
        src = sources[name]
        return src["context7_id"], src.get("default_tokens", DEFAULT_TOKENS)

    # Alias match
    for src_name, src in sources.items():
        aliases = src.get("aliases", [])
        if name in aliases:
            return src["context7_id"], src.get("default_tokens", DEFAULT_TOKENS)

    available = ", ".join(sorted(sources.keys()))
    raise Context7Error(f"Unknown source: {name}\nAvailable: {available}")


def query_documentation(source: str, topic: str = "", tokens: int | None = None) -> str:
    """Query Context7 API for documentation.

    Args:
        source: Source name or alias (e.g., "cpp", "openxr")
        topic: Topic to search for (e.g., "std::ranges")
        tokens: Maximum tokens to return (default from config)

    Returns:
        Documentation content as markdown string.
    """
    api_key = os.environ.get("CONTEXT7_API_KEY")
    if not api_key:
        raise Context7AuthError(
            "CONTEXT7_API_KEY environment variable not set.\n"
            "Get your API key at https://context7.com"
        )

    context7_id, default_tokens = resolve_source(source)
    tokens = tokens or default_tokens

    # Build request URL
    url = f"{API_BASE}/{context7_id}"
    params = {"tokens": str(tokens)}
    if topic:
        params["topic"] = topic
    query_string = urllib.parse.urlencode(params)
    full_url = f"{url}?{query_string}"

    # Make request
    headers = {
        "Authorization": f"Bearer {api_key}",
    }
    req = urllib.request.Request(full_url, headers=headers)

    try:
        with urllib.request.urlopen(req, timeout=TIMEOUT_SECONDS) as response:
            content = response.read().decode("utf-8")
            # API returns plain text markdown, but try JSON first for compatibility
            try:
                data = json.loads(content)
                return data.get("content", content)
            except json.JSONDecodeError:
                return content
    except urllib.error.HTTPError as e:
        body = e.read().decode("utf-8", errors="replace")
        if e.code == 401:
            raise Context7AuthError("Invalid API key") from e
        if e.code == 404:
            raise Context7NotFoundError(f"Library not found: {context7_id}") from e
        if e.code == 429:
            retry = e.headers.get("Retry-After")
            raise Context7RateLimitError(
                "Rate limit exceeded", retry_after=int(retry) if retry else None
            ) from e
        raise Context7Error(f"API error {e.code}: {body}") from e
    except urllib.error.URLError as e:
        raise Context7Error(f"Network error: {e.reason}") from e
