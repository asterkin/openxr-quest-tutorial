# OpenXR Quest Tutorial - Claude Code Guide

> AI Agent instructions for working with this repository

---

## Repository Purpose

This is a **tutorial and learning repository** containing consolidated OpenXR samples for **Meta Quest 3** development using **NDK + OpenXR**. This is NOT a production development repository.

**Production development** (InnoVision SDK, customer applications) happens in separate repositories.

---

## Quick Reference

- **Project Architecture**: [docs/Project_Plan.md](docs/Project_Plan.md) - Complete phase breakdown and architecture decisions
- **Environment Setup**: [docs/Environment_Setup.md](docs/Environment_Setup.md) - SDK installation guide (TBD)
- **Build/Deploy Guide**: [docs/Build_Deploy_Guide.md](docs/Build_Deploy_Guide.md) - Detailed workflows (TBD)

---

## Current Status

**Phase**: Repository Foundation (Phase 0/1)

**Active Work**: Initial repository setup and SDK documentation

**Next Steps**:
1. Document OpenXR SDK build process for Android
2. Document Meta XR SDK installation
3. Extract hello_xr sample from OpenXR-SDK-Source
4. Extract Meta samples from OVR SDK

---

## Repository Structure

```
openxr-quest-tutorial/                 # Quest-focused tutorial mono-repo
├── README.md                          # Human-readable overview
├── CLAUDE.md                          # This file - AI agent entry point
├── docs/                              # Detailed documentation
│   ├── Project_Plan.md                # Architecture and implementation plan
│   ├── Environment_Setup.md           # SDK installation (TBD)
│   ├── Build_Deploy_Guide.md          # Build/deploy workflows (TBD)
│   └── samples/                       # Sample analysis docs (TBD)
├── samples/                           # Sample projects (TBD)
│   ├── hello_xr/                      # Khronos OpenXR sample
│   └── meta/                          # Meta Quest samples
└── scripts/                           # Build/deploy automation (TBD)
```

---

## Environment Requirements

**System-Level SDKs** (installed globally, not in repo):

- **OpenXR SDK**: `C:\OpenXR-SDK` (must build from source for Android)
- **Meta XR SDK**: `C:\Meta-XR-SDK` (OVR OpenXR Mobile SDK 81.0)
- **Android NDK**: NDK r27 (via Android Studio)

**Environment Variables**:
```
OPENXR_SDK_ROOT=C:\OpenXR-SDK
META_XR_SDK_ROOT=C:\Meta-XR-SDK
ANDROID_NDK_ROOT=C:\Users\PRO\AppData\Local\Android\Sdk\ndk\27.x.x
```

---

## Target Platform

- **Device**: Meta Quest 3
- **OS**: Horizon OS v81 (Android API 34)
- **Graphics API**: Vulkan 1.0.3+
- **XR Runtime**: OpenXR 1.0.34
- **Build System**: CMake (native) + Gradle (Android)
- **Languages**: C/C++ (native), Java/Kotlin (Android layer)

---

## Working with This Repository

### Phase 0: SDK Installation (Current)

**Goal**: Document and verify computer-level SDK installations

**Tasks**:
1. Build OpenXR SDK from source for Android
2. Install/verify Meta XR SDK (OVR Mobile SDK 81.0)
3. Verify Android NDK r27 installation
4. Document installation process in `docs/Environment_Setup.md`
5. Create `scripts/setup_env.py` verification script

### Phase 1: Repository Foundation (Next)

**Goal**: Create base repository structure and documentation

**Tasks**:
1. ✅ Create repository structure
2. ✅ Create initial README.md
3. ✅ Create CLAUDE.md (this file)
4. ✅ Move planning document to `docs/Project_Plan.md`
5. Create root build configuration (settings.gradle, build.gradle)
6. Create .gitignore

### Future Phases

See [docs/Project_Plan.md](docs/Project_Plan.md) for complete phase breakdown:
- Phase 2: Extract hello_xr sample
- Phase 3: Extract Meta samples (19 samples)
- Phase 4: Build/deploy automation
- Phase 5: AI agent documentation

---

## Key Architecture Decisions

1. **Quest-Focused**: Primary target is Meta Quest 3 with NDK+OpenXR
2. **System-Level SDKs**: OpenXR and Meta XR SDKs installed globally, not in repo
3. **Build from Source**: OpenXR SDK must be built for Android (no prebuilts)
4. **Documentation-Driven**: AI agent instructions at root, detailed docs in `docs/`
5. **Separate Production**: InnoVision SDK and customer apps in separate repositories

---

## Documentation Standards

### File Locations

- **AI Agent Entry Point**: `CLAUDE.md` (this file, repo root)
- **Detailed Documentation**: `docs/` directory
- **Project Architecture**: `docs/Project_Plan.md`
- **Per-Sample Docs**: `samples/*/README.md` (quick reference)
- **Sample Analysis**: `docs/samples/` (deep dives)

### Cross-References

When referencing documentation:
- Use relative paths from current file location
- Link to specific sections with anchors when helpful
- Always provide context for cross-project references

---

## Git Workflow

**Repository Location**: `C:\Users\PRO\Projects\openxr-quest-tutorial\`

**Status**: New standalone repository (independent from InnoVision projects)

**Branch Strategy** (TBD):
- `main` - stable tutorial samples
- Feature branches for adding new samples or documentation

---

## Build System (Future)

**Multi-Project Gradle**:
- Root `settings.gradle` includes all samples as subprojects
- Each sample can be built independently or as part of batch build
- CMake for native layer, Gradle for Android packaging

**Automation Scripts** (TBD):
- `scripts/build.py` - Context-aware build script
- `scripts/deploy.py` - Quest 3 deployment script
- `scripts/setup_env.py` - Environment verification

---

## Version Information

- **Document Version**: 1.0
- **Created**: 2025-12-03
- **Last Updated**: 2025-12-03
- **Repository Status**: Initial setup
- **Current Phase**: Phase 0/1 - Foundation

---

**For detailed architecture and implementation plan, see [docs/Project_Plan.md](docs/Project_Plan.md)**
