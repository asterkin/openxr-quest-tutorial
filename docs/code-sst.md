# Code-SST: The LSP Complement

> **Status:** Conceptual Extension to Markdown-SST

## The Concept

While **Markdown-SST** structures the *Knowledge* of a project, **Code-SST** structures the *Implementation*. In a perfect agentic environment, these two graphs are traversed seamlessly.

A Language Server Protocol (LSP) server (like `clangd` or `pylance`) acts as the "Physics Engine" for the Code-SST graph, providing ground-truth data about relationships.

### Graph Mapping

| SST Element | C++ / Code Equivalent | Agent Tooling Analogy |
|-------------|-----------------------|-----------------------|
| **Node** | Symbol (Class, Function, Variable) | `grep "class MyClass"` |
| **CONTAINS** | Namespace, Class Scope, File | `read_file` (scope analysis) |
| **FOLLOWS** | Function Calls, `#include`, Inheritance | `grep` (call sites) |
| **NEAR** | Overloads, Friend Classes, Co-located Utils | `list_directory` |

## Micro-Kernel Logic (The Instruction Set)

To enable this without a running LSP process, we encode the "logic" of an LSP into the Agent's Root File (`GEMINI.md`).

**The Protocol:**
1.  **Define Target:** Identify the Symbol (Node) to modify.
2.  **Locate Node:** Find the Definition file.
3.  **Map Edges:**
    *   **In-Edges (Impact Analysis):** Who calls this? (Prevents breaking changes).
    *   **Out-Edges (Dependency Analysis):** What does this call? (Ensures correct usage).
4.  **Modify:** Only then, apply changes.

## Integration

This protocol is now embedded in `GEMINI.md` under "SST Navigation Protocol". It instructs the agent to simulate LSP behavior using `grep` and `read_file` to build a mental model of the dependency graph *before* writing code.
