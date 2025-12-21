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

## Shell Command Guidelines

**CRITICAL**: The Bash tool runs in Git Bash on Windows. PowerShell and cmd.exe are NOT available.

### Allowed Commands
- Pure Git Bash commands: `ls`, `cat`, `grep`, `rm`, `mkdir`, `cp`, `mv`, etc.
- Git commands: `git status`, `git add`, `git commit`, etc.
- Gradle wrapper: `./gradlew.bat` (runs via Git Bash)
- Python: `python` (if in PATH)

### NOT Allowed (will fail and waste tokens)
- `powershell` - Command not found
- `cmd.exe` - Command not found
- Windows-style paths in commands (use `/c/Users/...` not `C:\Users\...`)

### Path Conventions
```bash
# Windows path → Git Bash path
C:\Users\PRO\AppData\Local\Android\Sdk → /c/Users/PRO/AppData/Local/Android/Sdk
```

### Environment Variables
- Use `printenv` or `echo "$VAR"` (not `echo %VAR%`)
- Windows environment variables are inherited but use Unix syntax

---

## File Reading Guidelines

**Before reading unknown or potentially large files**, check the file size first to avoid token waste:

```bash
ls -lh /path/to/file  # Check size before reading
```

### Size Thresholds
| Size | Action |
|------|--------|
| < 500 lines | Read entire file |
| 500-2000 lines | Consider using `offset` and `limit` parameters |
| > 2000 lines | **Must chunk** - use `offset` and `limit` to read in sections |

### Chunked Reading Example
```
# First, check total lines
wc -l /path/to/file

# Then read in chunks
Read(file_path, offset=1, limit=500)    # Lines 1-500
Read(file_path, offset=500, limit=500)  # Lines 500-1000
```

### Best Practices
- Always check size of unfamiliar files (logs, generated code, data files)
- Use `Grep` to find relevant sections before reading entire files
- For config files and source code, full read is usually fine
- Large reads that fail waste significant tokens - prevention is key

---

## Quick Reference

- **Repository Structure**: [docs/Repository_Structure.md](docs/Repository_Structure.md) - Directory layout and organization
- **Project Plan**: [docs/Project_Plan.md](docs/Project_Plan.md) - Complete phase breakdown and current status
- **Environment Setup**: [docs/Environment_Setup.md](docs/Environment_Setup.md) - Target platform specs and SDK installation

---

## Documentation Standards

### File Locations

- **AI Agent Entry Point**: `CLAUDE.md` (this file, repo root)
- **Detailed Documentation**: `docs/` directory
- **Project Plan**: `docs/Project_Plan.md`
- **Project Architecture**: `docs/adrs/` (Architecture Decision Records)
- **Per-Sample Docs**: `samples/*/README.md` (quick reference)
- **Sample Analysis**: `docs/samples/` (deep dives)

### Cross-References

When referencing documentation:
- Use relative paths from current file location
- Link to specific sections with anchors when helpful
- Always provide context for cross-project references

---

## Required Skills for Common Tasks

### Architecture Decision Records (ADRs)

**IMPORTANT**: When any of these conditions apply, ALWAYS invoke the `adr` skill FIRST:

- User requests creating a new ADR
- User requests updating or superseding an existing ADR
- A decision is being made that affects system structure, dependencies, or build architecture
- Choosing between competing technical approaches with long-term implications
- Establishing new patterns or conventions for the repository

**Trigger phrases** (invoke skill immediately when you see these):
- "create an ADR", "new ADR", "document this decision"
- "update ADR", "supersede ADR"
- "we decided to", "the decision is" (when architecturally significant)

**ADR Location**: `docs/adrs/`
**Skill Location**: `.claude/skills/adr/`

The skill provides:
- Correct numbering sequence
- Standard template
- TOC update automation via `python .claude/skills/adr/scripts/update-adr-toc.py`

---

## Repository Navigation & Search Guidelines

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
