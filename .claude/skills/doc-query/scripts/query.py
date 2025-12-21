"""Query documentation from Context7.

Usage: python query.py <source> "<topic>" [tokens]

Examples:
    python query.py cpp "std::ranges views"
    python query.py openxr "XR_FB_passthrough"
    python query.py gradle "kotlin dsl" 3000
"""

import sys
from pathlib import Path

# Add lib directory to path
lib_dir = Path(__file__).parent.parent / "lib"
sys.path.insert(0, str(lib_dir))

from context7_client import Context7Error, query_documentation


def main() -> int:
    if len(sys.argv) < 3:
        print("Usage: python query.py <source> <topic> [tokens]", file=sys.stderr)
        print("\nExamples:", file=sys.stderr)
        print('  python query.py cpp "std::ranges"', file=sys.stderr)
        print('  python query.py openxr "passthrough"', file=sys.stderr)
        return 1

    source = sys.argv[1]
    topic = sys.argv[2]
    tokens = int(sys.argv[3]) if len(sys.argv) > 3 else None

    try:
        content = query_documentation(source, topic, tokens)
        print(content)
        return 0
    except Context7Error as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1
    except Exception as e:
        print(f"Unexpected error: {e}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
