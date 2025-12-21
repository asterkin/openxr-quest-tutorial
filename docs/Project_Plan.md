# OpenXR Quest Tutorial - Project Plan

> **Repository**: `openxr-quest-tutorial`
> **Target**: Meta Quest 3 (Horizon OS v81, Android API 34)

## Current Status

-   **Completed**: Phases 0, 1, 2, 3, 4, 5, and 6.
-   **In Progress**: Phase 7.
-   **Next**: Phase 8.

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

**Documentation Sources to Configure**:
| Source | Context7 ID | Purpose |
|--------|-------------|---------|
| C++ 20 | `websites/cppreference` | Modern C++ features, STL, language spec |
| CMake | `websites/cmake` | Build system, modules, toolchain files |
| Gradle | `websites/gradle` | Android build, AGP, Kotlin DSL |
| Android NDK | `websites/android_ndk` | Native development, JNI, toolchains |
| OpenXR | `github_repos/KhronosGroup/OpenXR-SDK-Source` | XR runtime API, extensions |
| Meta OpenXR | `github_repos/meta-quest/Meta-OpenXR-SDK` | Quest-specific extensions |
| Vulkan | `websites/vulkan` | Graphics API, SPIR-V, synchronization |

**Execution Plan**:
- [x] **Create ADR** - Document decision to adopt Context7 MCP for documentation access ([ADR-0011](adrs/adr-0011-use-context7-mcp-for-documentation-access.md))
- [ ] **Set up skill structure** - Create `.claude/skills/doc-query/` and `.claude/skills/add-doc/`
- [ ] **Implement doc-query skill** - Python script querying Context7 API with TOML config
- [ ] **Implement add-doc skill** - Script to register new documentation sources
- [ ] **Create doc-sources.toml** - Initial configuration with all 7 sources above
- [ ] **Update CLAUDE.md** - Add "Tool Documentation Access" section with usage triggers
- [ ] **Verify Context7 IDs** - Confirm all library IDs exist on context7.com
- [ ] **Test queries** - Validate each source returns relevant documentation
- [ ] **Document usage patterns** - Add examples for common scenarios (code explanation, migration)

### Phase 8: CI/CD Scripts
**Status**: ⏳ Planned
-   Implement automated build scripts triggered by git push to build all samples automatically.
