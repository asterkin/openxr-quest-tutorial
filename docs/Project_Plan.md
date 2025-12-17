# OpenXR Quest Tutorial - Project Plan

> **Repository**: `openxr-quest-tutorial`
> **Target**: Meta Quest 3 (Horizon OS v81, Android API 34)

## Current Status

-   **Completed**: Phases 0, 1, 2, and 3.
-   **In Progress**: Phase 4.
-   **Next**: Phase 5.

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
-   **Location**: `samples/hello_world/`

### Phase 3: `hello_xr` Sample
**Status**: ✅ Completed
-   The canonical Khronos `hello_xr` sample, adapted as a standalone project for Quest 3. Demonstrates a full rendering loop and multi-API support.
-   **Location**: `samples/hello_xr/`

### Phase 4: `openxr-tutorial` Series
**Status**: ✅ Completed
-   A progressive, multi-chapter tutorial series adapted from openxr-tutorial.com, demonstrating core OpenXR concepts from setup to advanced rendering and interactions.
-   **Location**: `samples/openxr-tutorial/`

### Phase 5: Meta SDK Samples
**Status**: 🔧 In Progress
-   Importing samples from the Meta OVR Mobile SDK to demonstrate Quest-specific features and OpenXR extensions (e.g., Passthrough, Hand Tracking, Spatial Anchors).
-   **Location**: `samples/meta/`

### Phase 6: Automation & Final Documentation
**Status**: ⏳ Planned
-   Development of Python-based automation scripts for building and deployment, and finalization of all project documentation.

## TODO
Upgarde to C++ 20.0:
  1. Update NDK to 25+ (install via Android Studio SDK Manager)
  2. Change Samples/CMakeLists.txt line 22 from:
  set(CMAKE_CXX_STANDARD 17)
  2. to:
  set(CMAKE_CXX_STANDARD 20)
  
