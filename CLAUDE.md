# OpenXR Quest Tutorial - Claude Code Guide

> AI Agent instructions for working with this repository

---

## Repository Purpose

This is a **tutorial and learning repository** containing consolidated OpenXR samples for **Meta Quest 3** development using **NDK + OpenXR**. This is NOT a production development repository.

**Production development** (InnoVision SDK, customer applications) happens in separate repositories.

---
## Interactive Session Guidelines
To save session context tokens space:
- Do not provide detailed summary for any request - only a short one
- Unless explicitly requested otherwise, respond with a mid-size text without going too deeply into details
- In case of doubt, ask calrifying questions before execution
- Do not jump to changes right after a problem investigation. Present your findings first.
---

## Quick Reference

- **Repository Structure**: [docs/Repository_Structure.md](docs/Repository_Structure.md) - Directory layout and organization
- **Project Plan**: [docs/Project_Plan.md](docs/Project_Plan.md) - Complete phase breakdown and current status
- **Environment Setup**: [docs/Environment_Setup.md](docs/Environment_Setup.md) - Target platform specs and SDK installation
- **Build/Deploy Guide**: [docs/Build_Deploy_Guide.md](docs/Build_Deploy_Guide.md) - Detailed workflows (TBD)

---

## Documentation Standards

### File Locations

- **AI Agent Entry Point**: `CLAUDE.md` (this file, repo root)
- **Detailed Documentation**: `docs/` directory
- **Project Plan**: `docs/Project_Plan.md`
- **Project Architecture**: `docs/adrs` (**TBD**)
- **Per-Sample Docs**: `samples/*/README.md` (quick reference)
- **Sample Analysis**: `docs/samples/` (deep dives)

### Cross-References

When referencing documentation:
- Use relative paths from current file location
- Link to specific sections with anchors when helpful
- Always provide context for cross-project references

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
