# Documentation Query Skill

Query up-to-date documentation for tools and SDKs via Context7 API.

## When to Use

Invoke this skill when:
- Using C++ 20, NDK, Gradle, CMake features beyond training cutoff
- Working with OpenXR or Meta XR SDK APIs
- Encountering errors with configured tools
- User asks "How do I..." questions about configured tools

## Usage

```bash
# List available documentation sources
python .claude/skills/doc-query/scripts/list-sources.py

# Query documentation
python .claude/skills/doc-query/scripts/query.py <source> "<topic>" [max_tokens]
```

## Examples

```bash
# Query C++ 20 ranges
python .claude/skills/doc-query/scripts/query.py cpp "std::ranges views"

# Query OpenXR extension
python .claude/skills/doc-query/scripts/query.py openxr "XR_FB_passthrough"

# Query Gradle Kotlin DSL
python .claude/skills/doc-query/scripts/query.py gradle "kotlin dsl android"
```

## Configuration

Documentation sources are defined in `.claude/doc-sources.toml`.

## Requirements

- Python 3.14+
- `CONTEXT7_API_KEY` environment variable
