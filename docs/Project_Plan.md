# OpenXR Quest Tutorial - Project Plan

> **Repository**: `openxr-quest-tutorial`
> **Target**: Meta Quest 3 (Horizon OS v81, Android API 34)

## Current Status

-   **Completed**: Phases 0, 1, 2, 3, 4, 5, 6, 7, and 8 (core functionality).
-   **In Progress**: Phase 8 outstanding TODOs (folder structure simplification).
-   **Next**: Phase 9.

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
**Status**: ✅ Completed
-   Upgraded toolchain and language standards to C++ 20, NDK 29, AGP 8.13.2, Gradle 8.13 across all samples.
-   Implemented composite builds, Gradle wrapper auto-bootstrap, and zombie icon cleanup.

### Phase 7: Connect to Context7 MCP
**Status**: 🔧 In Progress
-   Integrate with Context7 MCP (Model Context Protocol) to provide automated access to up-to-date documentation for all tools and SDKs.

**Approach**: Skills (not sub-agents)
-   Skills are user-invocable, lightweight, and proven in reference implementation
-   Sub-agents add complexity without significant benefit for documentation queries
-   Skills integrate naturally with CLAUDE.md workflow instructions

**Documentation Sources Configured** (verified 2025-12-21):
| Source | Context7 ID | Purpose |
|--------|-------------|---------|
| C++ | `isocpp/CppCoreGuidelines` | C++ Core Guidelines, modern best practices |
| CMake | `Kitware/CMake` | Build system, modules, toolchain files |
| Gradle | `gradle/gradle` | Android build, AGP, Kotlin DSL |
| Android NDK | `android/ndk` | Native development, JNI, toolchains |
| OpenXR | `KhronosGroup/OpenXR-SDK` | XR runtime API, extensions |
| Meta OpenXR | `meta-quest/Meta-OpenXR-SDK` | Quest-specific extensions |

**Execution Plan**:
- [x] **Create ADR** - Document decision to adopt Context7 MCP for documentation access ([ADR-0011](adrs/adr-0011-use-context7-mcp-for-documentation-access.md))
- [x] **Set up skill structure** - Create `.claude/skills/doc-query/` and `.claude/skills/add-doc/`
- [x] **Implement doc-query skill** - Python script querying Context7 API with TOML config
- [x] **Implement add-doc skill** - Script to register new documentation sources
- [x] **Document API key setup** - Add Context7 API key acquisition steps to [Environment_Setup.md](Environment_Setup.md)
- [x] **Create doc-sources.toml** - Initial configuration with all 6 sources above
- [x] **Update CLAUDE.md** - Add "Tool Documentation Access" section with usage triggers
- [x] **Verify Context7 IDs** - Confirmed all library IDs exist (removed unavailable: vulkan, gles)
- [x] **Test queries** - All 6 sources return documentation (topic filtering may 404 if no matches)
- [x] **Document usage patterns** - Added "Common Scenarios" section to CLAUDE.md

### Phase 8: NDK Camera2 + OpenXR + Vulkan Tutorial
**Status**: ✅ Core Functionality Complete
-   Build a minimal, full-screen Camera2 passthrough feed using the NDK Camera2 API (C++).
-   **Note**: An NDK C++ Camera2 tutorial does not exist; this will be built from first principles.
-   Use the existing OpenXR + Vulkan tutorial foundation for rendering and swapchain management.
-   **Location**: `openxr/camera2_tutorial/` (render loop, Vulkan setup, XR session lifecycle)

**Completed**:
-   [x] Create a new tutorial module with Gradle/CMake wiring and Android permissions.
-   [x] Add manifest features/permissions (`CAMERA`, `horizonos.permission.HEADSET_CAMERA`) and Quest 3/3S device gating.
-   [x] Implement NDK Camera2 device enumeration and select the headset passthrough camera.
-   [x] Create a capture session with a native output target (`AImageReader`).
-   [x] Define frame format/size (YUV 420 888 → RGBA conversion).
-   [x] Implement frame ingestion: acquire images, handle stride/format, and manage backpressure.
-   [x] Upload frames into a Vulkan image (staging buffer + layout transitions).
-   [x] Wire the Vulkan texture into OpenXR render loop using **Quad Layer** (head-locked, stereo-correct).
-   [x] Render full-screen camera feed with proper stereo fusion (both eyes see same image).

**Outstanding TODOs**:

| Priority | Issue | Description |
|----------|-------|-------------|
| Minor | Avatar body at startup | An avatar body briefly shows up at the beginning of the app |
| Nice-to-have | Video quality/latency | Low video quality and small delay; consider switching to "Vulkan YCbCr sampling + sampler conversion" if not too complex |
| **Must** | Simplify folder structure | Tutorial folder structure needs cleanup/simplification |

### Phase 9: CI/CD Scripts
**Status**: ⏳ Planned
-   Implement automated build scripts triggered by git push to build all samples automatically.
