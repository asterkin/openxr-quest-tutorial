#!/usr/bin/env python3
"""
Update the Architecture Decision Records (ADR) table of contents.

This script scans the docs/adrs/ directory for ADR markdown files,
extracts their metadata (number, title, status), and updates the
table of contents in docs/adrs/README.md.

Requirements: Python 3.12+
"""

import re
import sys
from pathlib import Path
from dataclasses import dataclass


@dataclass
class ADR:
    """Represents an Architecture Decision Record."""
    number: str
    filename: str
    title: str
    status: str

    def __lt__(self, other: object) -> bool:
        """Sort ADRs by number."""
        if not isinstance(other, ADR):
            return NotImplemented
        return self.number < other.number


def extract_adr_metadata(adr_file: Path) -> ADR | None:
    """
    Extract metadata from an ADR file.

    Args:
        adr_file: Path to the ADR markdown file

    Returns:
        ADR object with extracted metadata, or None if parsing fails
    """
    try:
        content = adr_file.read_text(encoding='utf-8')

        # Extract ADR number from filename (e.g., adr-0001-title.md -> 0001)
        number_match = re.match(r'adr-(\d{4})', adr_file.name)
        if not number_match:
            print(f"Warning: Could not extract number from {adr_file.name}", file=sys.stderr)
            return None
        number = number_match.group(1)

        # Extract title from H1 heading (e.g., "# ADR-0001: Title" -> "Title")
        title_match = re.search(r'^#\s+ADR-\d{4}:\s*(.+)$', content, re.MULTILINE)
        if not title_match:
            print(f"Warning: Could not extract title from {adr_file.name}", file=sys.stderr)
            return None
        title = title_match.group(1).strip()

        # Extract status (e.g., "**Status:** Accepted" -> "Accepted")
        # Handle multiline status like "Accepted - Phase 1 (Skill) - 2025-01-16"
        status_match = re.search(r'\*\*Status:\*\*\s*(.+?)(?:\n|$)', content, re.MULTILINE)
        if not status_match:
            print(f"Warning: Could not extract status from {adr_file.name}", file=sys.stderr)
            return None
        status = status_match.group(1).strip()

        return ADR(
            number=number,
            filename=adr_file.name,
            title=title,
            status=status
        )
    except Exception as e:
        print(f"Error processing {adr_file.name}: {e}", file=sys.stderr)
        return None


def find_adrs(adrs_dir: Path) -> list[ADR]:
    """
    Find and parse all ADR files in the directory.

    Args:
        adrs_dir: Path to the docs/adrs directory

    Returns:
        Sorted list of ADR objects
    """
    adrs: list[ADR] = []
    for adr_file in adrs_dir.glob('adr-*.md'):
        adr = extract_adr_metadata(adr_file)
        if adr is not None:
            adrs.append(adr)

    return sorted(adrs)


def generate_table(adrs: list[ADR]) -> str:
    """
    Generate the markdown table of contents.

    Args:
        adrs: List of ADR objects

    Returns:
        Markdown table as a string
    """
    if not adrs:
        return "| ADR Number | Title | Status |\n|------------|-------|--------|\n"

    table_lines = [
        "| ADR Number | Title | Status |",
        "|------------|-------|--------|"
    ]

    for adr in adrs:
        link = f"[ADR-{adr.number}]({adr.filename})"
        table_lines.append(f"| {link} | {adr.title} | {adr.status} |")

    return '\n'.join(table_lines)


def update_readme(readme_path: Path, new_table: str) -> None:
    """
    Update the README.md file with the new table of contents.

    Args:
        readme_path: Path to docs/adrs/README.md
        new_table: New table content as a string
    """
    content = readme_path.read_text(encoding='utf-8')

    # Find the table section (between "## Table of Contents" and next section or "---")
    # Pattern: Everything from table header to either "---" or end of file
    pattern = (
        r'(\| ADR Number \| Title \| Status \|\n'
        r'\|[-\s|]+\|\n'
        r'(?:\|[^\n]+\|\n)*)'
    )

    replacement_match = re.search(pattern, content)
    if not replacement_match:
        print("Error: Could not find table in README.md", file=sys.stderr)
        sys.exit(1)

    # Replace the old table with the new one
    updated_content = content.replace(replacement_match.group(1), new_table + '\n')

    readme_path.write_text(updated_content, encoding='utf-8')


def main():
    """Main entry point."""
    # Determine project root (script is in .claude/skills/adr/scripts/)
    script_dir = Path(__file__).parent
    skill_dir = script_dir.parent  # .claude/skills/adr
    claude_dir = skill_dir.parent  # .claude/skills
    claude_root = claude_dir.parent  # .claude
    project_root = claude_root.parent  # project root

    adrs_dir = project_root / 'docs' / 'adrs'
    readme_path = adrs_dir / 'README.md'

    # Validate paths
    if not adrs_dir.exists():
        print(f"Error: ADRs directory not found: {adrs_dir}", file=sys.stderr)
        sys.exit(1)

    if not readme_path.exists():
        print(f"Error: README not found: {readme_path}", file=sys.stderr)
        sys.exit(1)

    # Find and parse ADRs
    print(f"Scanning {adrs_dir} for ADR files...")
    adrs = find_adrs(adrs_dir)
    print(f"Found {len(adrs)} ADR(s)")

    # Generate new table
    new_table = generate_table(adrs)

    # Update README
    print(f"Updating {readme_path}...")
    update_readme(readme_path, new_table)

    print("✓ ADR table of contents updated successfully")

    # Print summary
    if adrs:
        print("\nCurrent ADRs:")
        for adr in adrs:
            print(f"  - ADR-{adr.number}: {adr.title} [{adr.status}]")


if __name__ == '__main__':
    main()
