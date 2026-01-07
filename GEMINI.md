# OpenXR Quest Tutorial - AI Agent Guide

> AI Agent instructions for working with this repository

---

## Repository Purpose

This is a **tutorial and learning repository** containing consolidated OpenXR samples for **Meta Quest 3** development using **NDK + OpenXR**. This is NOT a production development repository.

**Production development** (InnoVision SDK, customer applications) happens in separate repositories.

---
## Interactive Session Guidelines

To ensure efficient and clear interaction, please adhere to the following guidelines:

- **Be Concise:** Provide brief summaries and responses. Avoid lengthy explanations unless explicitly requested. This helps with focus and reduces cognitive load.
- **Clarify Ambiguity:** If a request is unclear, ask clarifying questions before proceeding with implementation.
- **Investigate First, Then Act:** For any non-trivial task, first investigate the existing code and documentation. Present your findings and a clear plan before implementing changes.
---

## Quick Reference

- **Repository Structure**: [docs/Repository_Structure.md](docs/Repository_Structure.md) - Directory layout and organization
- **Project Plan**: [docs/Project_Plan.md](docs/Project_Plan.md) - Complete phase breakdown and current status
- **Environment Setup**: [docs/Environment_Setup.md](docs/Environment_Setup.md) - Target platform specs and SDK installation

---

## Documentation Standards

### File Locations

- **AI Agent Entry Point**: `GEMINI.md` (this file, repo root)
- **Detailed Documentation**: `docs/` directory
- **Project Plan**: `docs/Project_Plan.md`
- **Project Architecture**: `docs/adrs` (**TBD**)
- **Per-Sample Docs**: `samples/*/README.md` (quick reference)
- **Sample Analysis**: `docs/samples/` (deep dives)

### Gemini-Specific Guidelines

To optimize interaction with this Gemini agent:

- **Utilize Tooling:** I have access to a variety of tools (e.g., `list_directory`, `read_file`, `replace`, `run_shell_command`, `codebase_investigator`). Feel free to request direct actions using these tools.
- **Provide Context:** For complex tasks, ensure all relevant file paths, function names, and desired changes are clearly specified. If unsure, allow me to investigate the codebase first using `codebase_investigator`.
- **Iterative Approach:** For larger tasks, prefer breaking them down into smaller, verifiable steps. This helps ensure accuracy and allows for intermediate feedback.

### Cross-References

When referencing documentation:
- Use relative paths from current file location
- Link to specific sections with anchors when helpful
- Always provide context for cross-project references

---

## Repository Navigation & Search Guidelines

### SST Navigation Protocol (Micro-Kernel)

**Concept**: Treat the repository as two complementary Semantic Spacetime (SST) graphs.
**Rule**: Always traverse the graph to "Locate" before "Reading".

#### 1. Documentation Graph (The Map)
*   **Structure**: Headers = Containers, Links = Edges.
*   **Protocol**:
    1.  **Entry**: Check `docs/Project_Plan.md` (or `docs/plan/current.md` if available) for context.
    2.  **Scan**: Use `read_file` (truncated) or `grep` on headers (`#`) to find relevant sections.
    3.  **Follow**: Traverse links to specific ADRs or guides. **Do not read the whole library.**

#### 2. Code Graph (The Territory)
*   **Structure**: Symbols = Nodes, References = Edges.
*   **Protocol (Simulated LSP)**:
    1.  **Symbol Search**: Use `grep` to find definition (`class X`, `void func`).
    2.  **In-Edges**: Use `grep` to find callers/usage.
    3.  **Out-Edges**: Check `#include` or imports.
    4.  **Action**: Only read the file once the specific "Node" (Symbol) is located.

### File Discovery Strategies

**Use appropriate tools for different search patterns:**

- **Glob** - Finding files by name/pattern:
  - CMake configs: `**/CMakeLists.txt`
  - Gradle files: `**/*.gradle`, `**/*.gradle.kts`
  - Native code: `**/*.cpp`, `**/*.h`, `**/*.c`
  - Documentation: `**/*.md`

- **Grep** - Searching code content:
  - OpenXR API calls: `xrCreate*`, `xrEnumerate*`, `xrGet*`
  - Vulkan calls: `vk*`, `VK_*`
  - Android/JNI: `JNI_OnLoad`, `JNIEXPORT`
  - Build configs: `android {`, `ndk {`

- **Read** - When you know exact file path or after Glob/Grep results

### Context-Aware Navigation

**Task-based documentation lookup:**

- Build issues → [docs/Build_Deploy_Guide.md](docs/Build_Deploy_Guide.md)
- SDK setup problems → [docs/Environment_Setup.md](docs/Environment_Setup.md)
- Architecture questions → [docs/Project_Plan.md](docs/Project_Plan.md)
- Sample-specific → `samples/{sample_name}/README.md`

### Repository Scope Reminders

- **Tutorial Repository**: Code prioritizes clarity over production patterns
- **Self-Contained Samples**: Each sample duplicates dependencies for learning
- **No Shared Libraries**: Samples don't share code between them
- **Documentation-First**: Always check docs/ before exploring code

### Pre-Commit Requirements

**CRITICAL**: Before any `git commit` or `git push`:
1. Update [docs/Repository_Structure.md](docs/Repository_Structure.md) if directory structure changed
2. Verify all cross-references in documentation are valid
3. Update relevant README.md files in affected samples

---

## Git Workflow

**Repository Location**: [`C:\Users\PRO\Projects\openxr-quest-tutorial\`](https://github.com/asterkin/openxr-quest-tutorial)

**Status**: New standalone public repository (independent from InnoVision projects)

**Branch Strategy**:
- `main` - stable tutorial samples
- Feature branches for adding new samples or documentation

---

