# OpenXR Tutorial Series

> Progressive tutorial samples adapted from [openxr-tutorial.com](https://openxr-tutorial.com/android/vulkan/1-introduction.html)

## Overview

This directory contains a series of tutorial samples that progressively introduce OpenXR concepts and implementation patterns for Meta Quest 3 development using Vulkan graphics API.

**Target Platform**: Meta Quest 3 (Horizon OS v81, Android API 34)
**Graphics API**: Vulkan 1.0+
**Build System**: Gradle + CMake with Maven dependencies

## Tutorial Structure

Each chapter builds upon the previous one, introducing new OpenXR concepts and implementation details:

### Table of Contents

| Chapter | Title | Status | Key Concepts |
|---------|-------|--------|--------------|
| [Chapter 1](chapter1/) | Introduction & Setup | 🔧 In Progress | Project structure, build configuration, Android initialization |
| Chapter 2 | TBD | ⏳ Planned | TBD |
| Chapter 3 | TBD | ⏳ Planned | TBD |
| Chapter 4 | TBD | ⏳ Planned | TBD |
| Chapter 5 | TBD | ⏳ Planned | TBD |

## Build System Integration

These tutorials follow the same build patterns as other samples in this repository:

- **OpenXR SDK Headers**: Fetched via CMake FetchContent from OpenXR-SDK-Source
- **OpenXR Loader**: Maven dependency (`org.khronos.openxr:openxr_loader_for_android`)
- **AndroidManifest.xml**: Quest 3-specific VR features and permissions
- **Gradle Configuration**: Consistent with repository-wide build settings

See [Build & Deploy Guide](../../docs/Build_Deploy_Guide.md) for details.

## Common Files

The tutorial series uses shared utility files located in each chapter's directory:

- **DebugOutput.h/cpp**: Console output redirection to Android logcat
- **HelperFunctions.h**: Platform-specific macros and utilities
- **OpenXRDebugUtils.h/cpp**: OpenXR debug extension (XR_EXT_debug_utils) setup
- **OpenXRHelper.h**: Consolidated OpenXR headers
- **GraphicsAPI.h/cpp**: Multi-API abstraction layer
- **GraphicsAPI_Vulkan.h/cpp**: Vulkan-specific implementation

## Differences from Original Tutorial

This adaptation differs from the original openxr-tutorial.com content in several ways:

1. **Quest 3 Focus**: Optimized for Meta Quest 3 with Horizon OS v81
2. **Maven Dependencies**: Uses Maven for OpenXR loader instead of manual builds
3. **CMake FetchContent**: Headers fetched from OpenXR-SDK-Source repository
4. **Standalone Samples**: Each chapter is a self-contained Gradle project
5. **Modern Build Tools**: AGP 8.13.0, Gradle 8.11.1, NDK r27

See individual chapter INTEGRATION_NOTES.md files for detailed changes.

## Prerequisites

Before running these tutorials, ensure you have:

- Android Studio (latest version recommended)
- Android SDK API 34
- Android NDK r27
- Java 17+
- Python 3.14+
- Meta Quest 3 with developer mode enabled

See [Environment Setup](../../docs/Environment_Setup.md) for detailed installation instructions.

## Quick Start

Each chapter can be built and deployed independently:

```bash
# Navigate to chapter directory
cd samples/openxr-tutorial/chapter1

# Build (from repository root)
gradlew :samples:openxr-tutorial:chapter1:assembleVulkanDebug

# Deploy to Quest 3
adb install -r app/build/outputs/apk/vulkanDebug/*.apk

# Run
adb shell am start -n [package.name]/.MainActivity
```

## Learning Path

**Recommended Approach**:

1. Start with [hello_world](../hello_world/) to understand basic Android/OpenXR setup
2. Review [hello_xr](../hello_xr/) for OpenXR SDK reference implementation
3. Follow openxr-tutorial chapters in sequence (Chapter 1 → Chapter 2 → ...)
4. Experiment with modifications to reinforce learning

## Documentation

- **Tutorial Source**: [openxr-tutorial.com Android/Vulkan](https://openxr-tutorial.com/android/vulkan/1-introduction.html)
- **OpenXR Specification**: [Khronos OpenXR Spec](https://www.khronos.org/registry/OpenXR/specs/1.0/html/xrspec.html)
- **Quest Development**: [Meta Quest Developer Hub](https://developer.oculus.com/documentation/native/)
- **Repository Docs**: [docs/](../../docs/)

## Support

For issues specific to these tutorial adaptations, please refer to:

- [Repository Structure](../../docs/Repository_Structure.md)
- [Build & Deploy Guide](../../docs/Build_Deploy_Guide.md)
- [Environment Setup](../../docs/Environment_Setup.md)

For questions about OpenXR concepts, consult the [original tutorial](https://openxr-tutorial.com/) or [OpenXR specification](https://www.khronos.org/registry/OpenXR/).

---

**Last Updated**: 2025-12-05
**Tutorial Series Status**: Phase 4 - In Progress
