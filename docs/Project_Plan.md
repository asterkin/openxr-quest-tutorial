# OpenXR Quest Tutorial - Project Plan

> **Repository**: `openxr-quest-tutorial`
> **Target**: Meta Quest 3 (Horizon OS v81, Android API 34)

## Current Status

-   **Completed**: Phases 0, 1, 2, 3, 4, and 5.
-   **In Progress**: Phase 6.
-   **Next**: Phase 7.

## Implementation Phases

### Phase 0: Prerequisites
**Status**: ✅ Completed
-   Development tools installed and environment configured. OpenXR SDK is managed automatically via Gradle/CMake.

### Phase 1: Repository Foundation
**Status**: ✅ Completed
-   The core repository structure, documentation framework, and build system foundations are in place.

### Phase 2: `hello_world` Sample
**Status**: ✅ Completed
-   A minimal OpenXR sample to verify basic environment setup and instance creation.
-   **Location**: `openxr/hello_world/`

### Phase 3: `hello_xr` Sample
**Status**: ✅ Completed
-   The canonical Khronos `hello_xr` sample, adapted as a standalone project for Quest 3. Demonstrates a full rendering loop and multi-API support.
-   **Location**: `openxr/hello_xr/`

### Phase 4: `openxr-tutorial` Series
**Status**: ✅ Completed
-   A progressive, multi-chapter tutorial series adapted from openxr-tutorial.com, demonstrating core OpenXR concepts from setup to advanced rendering and interactions.
-   **Location**: `openxr/tutorial/`

### Phase 5: Meta SDK Samples Migration
**Status**: ✅ Completed
-   Samples from the Meta OVR Mobile SDK have been imported and integrated to demonstrate Quest-specific features and OpenXR extensions (e.g., Passthrough, Hand Tracking, Spatial Anchors).
-   **Location**: `meta/`

### Phase 6: Upgrade to Latest Tool Versions
**Status**: 🔧 In Progress
-   Upgrade the toolchain and language standards across the projects to more recent versions. This includes:
    1.  Upgrading to C++ 20.
    2.  Updating the Android NDK to version 25 or newer.
    3.  Streamlining Gradle, NDK, and C++ versions across all samples for consistency.
    4.  Adding auto-cleaup to meta build.gradle to remove zomby icons

**Implementation Plan**:
- [x] **Audit Current State** - Inventory C++ standards, NDK versions, Gradle/AGP versions across all samples
- [ ] **Define Target Versions** - Finalize C++ 20, NDK (25/26/27), Gradle, AGP, and SDK versions
- [ ] **Upgrade Pilot Sample** - Update `openxr/hello_world/` first, build and verify on Quest 3
- [ ] **Propagate to OpenXR Samples** - Update `openxr/hello_xr/` and `openxr/tutorial/` chapters
- [ ] **Propagate to Meta Samples** - Update all samples in `meta/`
- [ ] **Standardize Build Configuration** - Extract common version properties, ensure consistency
- [ ] **Meta Auto-Cleanup** - Add Gradle task to remove zombie icons
- [ ] **Final Verification** - Build and test all samples on Quest 3
- [ ] **Update Documentation** - Update `docs/Environment_Setup.md` with new standard versions
- [ ] **Clean Up Unused Versions** - Remove old NDK versions, unused Gradle distributions, and deprecated dependencies

**Audit Results** (2025-12-18):

| Component | hello_world | hello_xr | tutorial | meta samples |
|-----------|-------------|----------|----------|--------------|
| **AGP** | 8.13.0 | 8.13.0 | 8.10.1 | 7.0.3 |
| **Gradle** | 8.13 | 8.13 | 8.12 | 7.2 |
| **NDK** | 27.2.12479018 | 27.2.12479018 | 27.2.12479018 | (not set) |
| **C++ Standard** | 17 | 17 | 17 | 17 |
| **CMake** | 3.22.1 | 3.22.1 | 3.22.1+ | 3.10.2 |
| **compileSdk** | 34 | 34 | 34 | 32 |
| **minSdk** | 29 | 29 | 29 | 26 |
| **targetSdk** | 34 | 34 | 34 | 32 |
| **Java** | 17 | 17 | 17 | (default) |
| **OpenXR Loader** | 1.1.54 (Maven) | FetchContent | 1.1.54 (Maven) | 1.1.51 (Maven) |

**Key Findings**:
- OpenXR samples (`hello_world`, `hello_xr`, `tutorial`) are mostly aligned with modern tooling
- Meta samples are significantly outdated (AGP 7.0.3, Gradle 7.2, SDK 32)
- All samples currently use C++ 17 (target: C++ 20)
- NDK 27 already in use for OpenXR samples; meta samples lack explicit NDK version

### Phase 7: Connect to Context7 MCP
**Status**: ⏳ Planned
-   Integrate with the Context7 MCP (Mission Control Platform) to automate the generation and maintenance of up-to-date project documentation based on the codebase.

### Phase 8: CI/CD Scripts
**Status**: ⏳ Planned
-   Implement automated build scripts triggered by git push to build all samples automatically.
