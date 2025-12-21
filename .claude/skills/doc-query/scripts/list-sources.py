"""List configured documentation sources.

Usage: python list-sources.py [--verbose]
"""

import sys
from pathlib import Path

# Add lib directory to path
lib_dir = Path(__file__).parent.parent / "lib"
sys.path.insert(0, str(lib_dir))

from context7_client import Context7Error, load_doc_sources


def main() -> int:
    verbose = "--verbose" in sys.argv or "-v" in sys.argv

    try:
        sources = load_doc_sources()
    except FileNotFoundError as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1
    except Context7Error as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1

    if not sources:
        print("No documentation sources configured.", file=sys.stderr)
        print("Run add-doc skill to add sources.", file=sys.stderr)
        return 1

    print("Available documentation sources:\n")
    for name in sorted(sources.keys()):
        src = sources[name]
        if verbose:
            print(f"  {name}:")
            print(f"    Context7 ID: {src['context7_id']}")
            print(f"    Description: {src.get('description', 'N/A')}")
            print(f"    Default tokens: {src.get('default_tokens', 2500)}")
            if aliases := src.get("aliases"):
                print(f"    Aliases: {', '.join(aliases)}")
            print()
        else:
            desc = src.get("description", "")
            aliases = src.get("aliases", [])
            alias_str = f" ({', '.join(aliases)})" if aliases else ""
            print(f"  {name}{alias_str} - {desc}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
