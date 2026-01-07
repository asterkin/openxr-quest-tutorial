# Markdown-SST: Dynamic Knowledge Graphs from Documentation

> **Attribution:** This technique for dynamically deriving a Semantic Spacetime (SST) graph directly from standard Markdown structures was discovered and proposed by **Gemini** (January 2026).

## The Core Concept

Traditional knowledge graphs require maintaining separate metadata files (RDF, YAML, JSON), which inevitably drift out of sync with the actual documentation. **Markdown-SST** eliminates this redundancy by treating the **document structure itself** as the graph.

By parsing standard Markdown syntax, an AI agent can reconstruct the repository's semantic topology on the fly:

### 1. Hierarchy = CONTAINS
The nested header structure of a Markdown file defines the `CONTAINS` relationship.
*   `# Title` contains `## Section`
*   `## Section` contains `### Subsection`

**Agent Behavior:** To "zoom in" on a topic, an agent scans for the relevant header depth. It does not need to read the entire file to know that "Phase 7" is part of the "Project Plan".

### 2. Hyperlinks = FOLLOWS / NEAR
Links represent directed edges between nodes (documents or sections). The relationship type is inferred from the surrounding context:

*   **FOLLOWS (Dependency):** Links preceded by "Based on", "Implements", "Depends on", or links pointing to prerequisite documents (e.g., `[Phase 7](../Project_Plan.md)`).
*   **NEAR (Reference):** Links preceded by "See also", "Reference", or general context links (e.g., `[OpenXR Specification](https://...)`).

**Agent Behavior:** An agent traversing the graph follows these "portals" to discover dependencies or related context without hallucinating connections.

---

## Reference Implementation

The following Python script demonstrates how to parse a Markdown file into an SST node structure. This logic can be embedded directly into an agent's reasoning loop or codified as a tool.

```python
import re
import sys
import yaml
from pathlib import Path

def scan_file(file_path):
    path = Path(file_path)
    if not path.exists():
        return {"error": "File not found"}

    content = path.read_text(encoding='utf-8')
    lines = content.splitlines()

    root = {
        "node": path.name,
        "path": str(path),
        "contains": [],
        "follows": [],
        "near": []
    }

    # Stack to manage hierarchy: [(level, dict_ref)]
    # We use a dummy root at level 0
    stack = [(0, root["contains"])]

    # Regex patterns
    header_pattern = re.compile(r'^(#+)\s+(.*)')
    link_pattern = re.compile(r'\[([^\]]+)\]\(([^)]+)\)')
    
    for i, line in enumerate(lines):
        line = line.strip()
        
        # 1. Parse Headers (CONTAINS)
        header_match = header_pattern.match(line)
        if header_match:
            level = len(header_match.group(1))
            title = header_match.group(2).strip()
            
            new_node = {
                "name": title,
                "type": "section",
                "contains": [],
                "follows": [] # Section-level links will be added here
            }
            
            # Pop stack until we find a parent with level < current level
            while stack and stack[-1][0] >= level:
                stack.pop()
            
            # Add to parent
            if stack:
                stack[-1][1].append(new_node)
            
            # Push new node
            stack.append((level, new_node["contains"]))
            continue

        # 2. Parse Links (FOLLOWS / NEAR)
        for match in link_pattern.finditer(line):
            text = match.group(1)
            target = match.group(2)
            
            # Context analysis for edge typing
            relation = "follows" # Default assumption
            pre_context = line[:match.start()].lower()
            
            if "see also" in pre_context or "references" in pre_context:
                relation = "near"
            elif "based on" in pre_context or "implements" in pre_context or "depends on" in pre_context:
                relation = "follows"
            
            # Add link node to the current section
            stack[-1][1].append({
                "relation": relation,
                "target": target,
                "text": text,
                "context": pre_context.strip()
            })

    return root

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python sst_scanner.py <file_path>")
        sys.exit(1)
        
    result = scan_file(sys.argv[1])
    print(yaml.dump(result, sort_keys=False, default_flow_style=False))
```

## Advantages

1.  **Zero Maintenance:** The graph updates automatically as you write documentation. There is no separate "graph database" to sync.
2.  **Human-Readable:** The source of truth is the documentation humans read, ensuring alignment between the agent's understanding and the developer's intent.
3.  **Lazy Loading:** An agent can scan the headers of `Project_Plan.md` (Token Cost: Low) to find the relevant phase, then only read the detailed ADR linked from that phase (Token Cost: High) when necessary.

## SST-Folder Structure (The Temporal Linked List)

A monolithic `Project_Plan.md` eventually suffers from "context rot"—detailed history is deleted to make room for new phases. The **SST-Folder** strategy resolves this by decoupling **State** (Process) from **Definition** (Agent Code).

### The Architecture
*   **Definition:** `GEMINI.md` / `CLAUDE.md`. Static instructions. The "Kernel".
*   **State:** `docs/plan/`. Dynamic process state. The "RAM".

### The Directory Model
This structure creates a linear narrative that scales indefinitely while maintaining token economy.

```text
docs/plan/
├── 00-overview.md        # The "Map": High-level milestones (The "Contains" view)
├── 01-prerequisites.md   # Frozen History (Detailed record of past work)
├── 02-hello-world.md     # Frozen History
├── ...
├── 09-ci-cd.md           # ACTIVE (Detailed steps, unchecked boxes)
└── current.md            # THE POINTER (Symlink-like behavior)
```

### The `current.md` Pointer
This file acts as the entry point for the agent's "Where am I?" query.

**Content:**
```markdown
# Current Status
**Active Phase:** [Phase 9: CI/CD Scripts](09-ci-cd.md)
**State:** Planning
**Blockers:** None
```

**Workflow:**
1.  **Locate:** Agent reads `current.md` → finds link to `09-ci-cd.md`.
2.  **Load:** Agent reads *only* the active phase file.
3.  **Result:** Zero tokens spent on Phases 1-8 unless historical context is explicitly requested (via traversal of `00-overview.md`).

### Advantages over Issue Trackers
1.  **Linear Narrative:** Enforces a coherent "story" of the project, which LLMs process more effectively than a "bag of issues."
2.  **Historical Integrity:** Past phases remain detailed and accessible (via file system traversal) without clogging the active context window.
3.  **Scalability:** The plan can grow to hundreds of phases without impacting the cost of determining the *current* step.

---

## Codex Comments

These notes assume the approach is exploratory and should stay lightweight.

**What works well**
- Using headers and links as the graph avoids drift and aligns with "search before read."
- The pointer-first idea keeps context cost low when docs grow.

**Risks / gaps**
- The `current.md` pointer adds a new artifact and workflow; without strict adoption it goes stale.
- Link typing ("see also" = NEAR) is brittle; docs vary in phrasing.
- The approach assumes a stable "current state," which may not exist in user-driven sessions.

**Practical alternative (no new files)**
- Treat "In Progress" text markers as the pointer:
  - `rg "In Progress" docs\\Project_Plan.md`
  - If none, pick the lowest-numbered "Planned/Not Started" item.
- When advancing, mark previous as "Done" and next as "In Progress."

**Use in this repo**
- Fits as a navigation protocol, not a full framework.
- Keep it opt-in until a dedicated repo formalizes the process.

**Extraction checklist (future repo)**
- Define the minimal artifacts (e.g., `plan/current.md` or "In Progress" markers).
- Specify the exact search commands and fallback rules.
- Provide one real repo example with paths and links.
- Keep the protocol under one page; defer theory to separate docs.
