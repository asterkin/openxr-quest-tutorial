# GEMINI-SST: Semantic Spacetime Model for Gemini Agent Optimization

## Declaration of Intent

This document adapts the **Semantic Spacetime (SST)** conceptual model—originally developed for Claude—to the **Gemini CLI** environment. It serves as a blueprint for maximizing agent efficiency, accuracy, and token economy within this repository.

## Core Philosophy: The Gemini Graph

The Gemini agent operates within a **knowledge graph** constructed from the repository's state. Unlike a static analysis tool, this graph is explored dynamically.

### 1. The Entry Point: `GEMINI.md`
Just as `CLAUDE.md` serves as the entry point for Claude, `GEMINI.md` is the **Root Node** for my session context.
- **Function:** It establishes the "Physics" of the session (Mandates, Workflows) and provides the initial "Map" (Directory Structure, Key Docs).
- **Optimization:** It must remain concise, functioning as a *routing table* rather than a *knowledge base*. It points to where information is (using relative paths), rather than containing it all.

### 2. Connection Types (The Vocabulary)
The four SST connection types are universally applicable:

| Type | Gemini Interpretation | Tooling Mapping |
|------|-----------------------|-----------------|
| **CONTAINS** | Directory listing, file structure | `list_directory`, `glob` |
| **FOLLOWS** | Imports, dependencies, build order | `search_file_content` (grep), `read_file` |
| **EXPRESSES** | Documentation, comments, headers | `read_file` |
| **NEAR** | Co-located files, naming conventions | `list_directory` (sibling files) |

### 3. Tool-Driven Traversal
My navigation of this graph is defined by my toolset. Efficient traversal minimizes "Token Hops" (expensive reads).

*   **Scan (Low Cost):** `list_directory`, `glob`. These reveal the `CONTAINS` edges without loading content.
*   **Probe (Medium Cost):** `search_file_content`. This reveals `FOLLOWS` and `NEAR` edges (references, usage) without reading full files.
*   **Read (High Cost):** `read_file`. This traverses an `EXPRESSES` edge to load the actual node content into context. **Strategy:** Only read the destination node after confirming it's the correct target via Scan/Probe.

## Token Economics & Memory Strategy

### The "Investigate First" Protocol
The directive "Investigate First, Then Act" is essentially an instruction to **build the local graph** before attempting to modify it.
1.  **Ping:** Check existence (`glob`).
2.  **Map:** Understand surroundings (`list_directory`).
3.  **Verify:** Confirm content (`read_file`, `search_file_content`).
4.  **Act:** Modify (`replace`, `write_file`).

### Long-Term Memory (The Shortcut Edge)
Unlike Claude's ephemeral context described in the original SST doc, I possess a `save_memory` tool.
- **Function:** This creates a **persistent, zero-cost edge** in the graph.
- **Usage:** If I discover a complex relationship (e.g., "The build fails if X is missing in Y"), I can save this fact. In future sessions, this fact is "pre-loaded," allowing me to skip the traversal steps required to rediscover it.

## Implementation Guide

### For the User (How to help me)
1.  **Curate `GEMINI.md`:** Keep it updated. If the directory structure changes, update the "Map".
2.  **Use specific pointers:** When prompting, referencing a specific file acts as a direct "Teleport" to that node, saving traversal tokens.

### For the Agent (How I optimize)
1.  **Prefer `search_file_content` over `read_file` for discovery:** Don't read a file just to see if it contains a string.
2.  **Breadth-First for Structure:** Use `list_directory` to understand layout before diving deep.
3.  **Lazy Loading:** Do not read `docs/` unless the user's query specifically requires semantic understanding of the project plan or architecture.

---

# Appendix: Critical Review of `claude-sst.md`

## 1. Analysis of the SST Framework
The "Semantic Spacetime" model proposed in `claude-sst.md` is a sophisticated attempt to formalize the interaction between an LLM and a codebase.

**Strengths:**
*   **Formalized Context Management:** The distinction between "Static Graph" (Codebase) and "Dynamic Graph" (Session) is excellent. It highlights that the agent's "understanding" is a temporary overlay on the permanent code.
*   **Lazy Loading:** The idea of hierarchical `CLAUDE.md` files (Layer 0, 0.5, 1...) is a crucial architectural pattern for large repositories. It prevents context window flooding.
*   **Universal Vocabulary:** Reducing relationships to `CONTAINS`, `FOLLOWS`, `EXPRESSES`, `NEAR` simplifies the mental model for the agent.

**Weaknesses:**
*   **Over-Engineering:** The "Stochastic RISC" and "VM" analogies, while intellectually stimulating, perhaps overcomplicate the practical reality. The agent doesn't need to "simulate a VM"; it just needs to be an efficient reader and writer.
*   **Tool-Specificity:** The original document is heavily coupled to specific Claude CLI features (`/compact`, `skills`, specific slash commands). These are implementation details, not fundamental principles of AI software engineering.

## 2. Applicability to Gemini
The core *principles* of SST are highly applicable to Gemini, even if the *mechanisms* differ.

*   **Routing:** `GEMINI.md` performs the exact same function as `CLAUDE.md`. It is the "Root Node".
*   **Token efficiency:** The "Mini-Max" goal (maximize accuracy, minimize tokens) is universal.
*   **Differences:**
    *   **Memory:** Gemini's `save_memory` tool allows for a "Learned Graph" that persists across sessions, arguably offering a superior mechanism to the "Rules" layer described for Claude, which must be re-read every time.
    *   **Search Power:** Gemini's `search_file_content` (ripgrep) and `google_web_search` provide stronger "Discovery" capabilities, possibly reducing the need for strictly pre-defined navigation paths (like "Skills" as rigid entry points). We can "Find" our way rather than just "Follow" pre-set paths.

## 3. Conclusion
The SST approach is a valid and useful mental model. For this repository, we will adopt the **functional** aspects (Hierarchical Documentation, Entry Point Routing, Investigation-First) without necessarily adopting the dense **theoretical** terminology (Stochastic RISC, Promise Theory) unless it serves a practical simplification.

**Recommendation:** Maintain `GEMINI.md` as the "Routing Table" and encourage "Lazy Loading" of documentation.
