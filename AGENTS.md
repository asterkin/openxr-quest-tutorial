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

- **AI Agent Entry Point**: `AGENTS.md` (this file, repo root)
- **Other Agent Files**: `CLAUDE.md`, `GEMINI.md` (kept roughly in sync, but `AGENTS.md` is canonical for agent tooling)
- **Detailed Documentation**: `docs/` directory
- **Project Plan**: `docs/Project_Plan.md`
- **Project Architecture**: `docs/adrs/`
- **Per-Sample Docs**: `openxr/*/README.md`, `openxr/tutorial/README.md`, `meta/README.md` (quick reference)
- **Sample Analysis**: `docs/adrs/` (deep dives; sample-specific analyses may be added later)

### Cross-References

When referencing documentation:
- Use relative paths from current file location
- Link to specific sections with anchors when helpful
- Always provide context for cross-project references

---

## Repository Navigation & Search Guidelines

### File Discovery Strategies

**Use appropriate tools for different search patterns:**

### Preferred Commands (Windows `cmd.exe`)

This repo is often used from a Windows `cmd` shell in agent harnesses. Prefer these commands to avoid tool availability issues:

**Known-working commands in this environment:**
- `dir` (including `/b`, `/s`, `/a`)
- `type`
- `copy`
- `mkdir`
- `rg` / `rg --files` (fast content search)
- `cmd /c` (wraps commands to avoid quoting issues in agent harnesses)

**Known-unavailable commands in this environment:**
- `powershell`
- `xcopy`
- `robocopy`
- `findstr`
- `find`
- `more`

- **List files by pattern (glob)**: `dir /s /b *.md`, `dir /s /b **\CMakeLists.txt` (use folder wildcards where helpful)
- **List a directory**: `dir`, `dir /a` (shows hidden), `dir /s` (recursive)
- **Read a file**: `type path\to\file.md` (no pager available in this environment)
- **Find files by name substring**: `dir /s /b *OpenXR*`
- **Fast content search**: `rg "pattern"` (if installed), fallback to Python one-liner (see below)
- **Avoid**: `<<` heredocs and bash-style pipes/flags that `cmd` doesn’t support

### Python Usage (avoid quoting pitfalls)

When shell tools like `rg/where/fc` aren’t available, use Python. In `cmd.exe`, avoid multi-line `python -` input; prefer `python -c` one-liners or write a temporary `.py` file.

- **Run a one-liner** (keep it a single line; use Python `;` to separate statements; wrap with `cmd /c`):
  - `cmd /c python -c "import pathlib; print('\n'.join(map(str, pathlib.Path('docs').rglob('*.md'))))"`
- **Print file with line numbers** (single-line list-comprehension to avoid indentation issues):
  - `cmd /c python -c "import pathlib; lines=pathlib.Path('AGENTS.md').read_text(encoding='utf-8',errors='replace').splitlines(); [print(f'{i:4d}: {l}') for i,l in enumerate(lines,1)]"`
- **Quick content search fallback** (example: search `xrCreate` in `*.cpp`):
  - `cmd /c python -c "import pathlib,re; pat=re.compile(r'xrCreate'); [print(f'{p}:{i+1}:{line}') for p in pathlib.Path('.').rglob('*.cpp') for i,line in enumerate(p.read_text(errors='ignore').splitlines()) if pat.search(line)]"`

- **If quoting gets messy**, write a temporary script instead:
  - `cmd /c (echo import pathlib & echo print('\\\\n'.join(map(str, pathlib.Path('docs').rglob('*.md'))))) > tmp_list.py`
  - `cmd /c python tmp_list.py`
- **If Python output seems missing**, redirect to a file and `type` it:
  - `cmd /c python -c "print('hello')" > tmp_out.txt`
  - `type tmp_out.txt`
- **Cleanup temp scripts/files when done**:
  - `cmd /c del tmp_list.py tmp_out.txt`

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

- Build & deploy → [openxr/Build_Guidelines.md](openxr/Build_Guidelines.md) and [meta/README.md](meta/README.md)
- Build system architecture → [docs/adrs/adr-0007-build-and-run-architecture.md](docs/adrs/adr-0007-build-and-run-architecture.md)
- SDK setup problems → [docs/Environment_Setup.md](docs/Environment_Setup.md)
- Architecture questions → [docs/Project_Plan.md](docs/Project_Plan.md)
- Sample-specific → `openxr/{sample_name}/README.md` or `meta/Samples/XrSamples/{sample_name}/README.md`

### Repository Scope Reminders

- **Tutorial Repository**: Code prioritizes clarity over production patterns
- **Self-Contained Samples**: Each sample duplicates dependencies for learning
- **No Shared Libraries**: Samples don't share code between them
- **Documentation-First**: Always check docs/ before exploring code
- **Generated Build Outputs**: Avoid editing `**/.cxx/` and `**/build/` (safe to delete when troubleshooting)

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
