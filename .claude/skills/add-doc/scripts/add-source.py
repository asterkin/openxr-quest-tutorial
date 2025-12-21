"""Add a documentation source to .claude/doc-sources.toml.

Usage: python add-source.py <name> <context7_id> "<description>" [--tokens N] [--alias NAME]

Examples:
    python add-source.py cpp "websites/cppreference" "C++ 20 language and STL"
    python add-source.py ndk "websites/android_ndk" "Android NDK" --tokens 3000
    python add-source.py openxr "github_repos/KhronosGroup/OpenXR-SDK-Source" "OpenXR API" --alias xr
"""

import sys
import tomllib
from pathlib import Path


class AddSourceError(Exception):
    """Error adding documentation source."""


def find_project_root() -> Path:
    """Find project root by looking for .claude directory."""
    current = Path.cwd()
    for parent in [current, *current.parents]:
        if (parent / ".claude").is_dir():
            return parent
    raise AddSourceError("Cannot find project root (.claude directory)")


def load_existing_sources(config_path: Path) -> dict:
    """Load existing sources from config file."""
    if not config_path.exists():
        return {}
    with open(config_path, "rb") as f:
        data = tomllib.load(f)
    return data.get("sources", {})


def validate_source_name(name: str, existing: dict) -> None:
    """Check that source name doesn't conflict."""
    if name in existing:
        raise AddSourceError(f"Source '{name}' already exists")
    # Check if name conflicts with existing aliases
    for src_name, src in existing.items():
        if name in src.get("aliases", []):
            raise AddSourceError(f"Name '{name}' conflicts with alias of '{src_name}'")


def validate_aliases(aliases: list[str], name: str, existing: dict) -> None:
    """Check that aliases don't conflict."""
    for alias in aliases:
        if alias in existing:
            raise AddSourceError(f"Alias '{alias}' conflicts with existing source")
        if alias == name:
            raise AddSourceError(f"Alias '{alias}' same as source name")
        for src_name, src in existing.items():
            if alias in src.get("aliases", []):
                raise AddSourceError(f"Alias '{alias}' already used by '{src_name}'")


def format_toml_entry(name: str, context7_id: str, description: str,
                      tokens: int, aliases: list[str]) -> str:
    """Format a TOML entry for a documentation source."""
    lines = [f"[sources.{name}]"]
    lines.append(f'context7_id = "{context7_id}"')
    lines.append(f'description = "{description}"')
    lines.append(f"default_tokens = {tokens}")
    if aliases:
        alias_str = ", ".join(f'"{a}"' for a in aliases)
        lines.append(f"aliases = [{alias_str}]")
    return "\n".join(lines) + "\n"


def append_source_to_config(config_path: Path, entry: str) -> None:
    """Append a new source entry to the config file."""
    # Create file with header if it doesn't exist
    if not config_path.exists():
        config_path.parent.mkdir(parents=True, exist_ok=True)
        header = """# Documentation Sources Configuration
# Maps tools and SDKs to their Context7 documentation sources.
# Used by doc-query skill for accessing up-to-date documentation.

"""
        config_path.write_text(header + entry)
    else:
        with open(config_path, "a") as f:
            f.write("\n" + entry)


def parse_args(args: list[str]) -> tuple[str, str, str, int, list[str]]:
    """Parse command line arguments."""
    if len(args) < 3:
        raise AddSourceError(
            "Usage: python add-source.py <name> <context7_id> <description> "
            "[--tokens N] [--alias NAME]"
        )

    name = args[0]
    context7_id = args[1]
    description = args[2]
    tokens = 2500
    aliases = []

    i = 3
    while i < len(args):
        if args[i] == "--tokens" and i + 1 < len(args):
            tokens = int(args[i + 1])
            i += 2
        elif args[i] == "--alias" and i + 1 < len(args):
            aliases.append(args[i + 1])
            i += 2
        else:
            raise AddSourceError(f"Unknown argument: {args[i]}")

    return name, context7_id, description, tokens, aliases


def main() -> int:
    try:
        name, context7_id, description, tokens, aliases = parse_args(sys.argv[1:])

        project_root = find_project_root()
        config_path = project_root / ".claude" / "doc-sources.toml"

        existing = load_existing_sources(config_path)
        validate_source_name(name, existing)
        if aliases:
            validate_aliases(aliases, name, existing)

        entry = format_toml_entry(name, context7_id, description, tokens, aliases)
        append_source_to_config(config_path, entry)

        print(f"Added documentation source: {name}")
        print(f"  Context7 ID: {context7_id}")
        print(f"  Description: {description}")
        print(f"  Default tokens: {tokens}")
        if aliases:
            print(f"  Aliases: {', '.join(aliases)}")
        print(f"\nConfig: {config_path}")
        return 0

    except AddSourceError as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1
    except Exception as e:
        print(f"Unexpected error: {e}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
