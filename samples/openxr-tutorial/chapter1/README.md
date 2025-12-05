# Chapter 1: Introduction & Setup

> OpenXR Tutorial Chapter 1 adapted for Meta Quest 3

## Overview

This chapter introduces OpenXR fundamentals and establishes the project structure for Android/Vulkan development on Quest 3.

**Status**: 🔧 In Progress

## Original Tutorial

This sample is adapted from:
- **Source**: [openxr-tutorial.com - Chapter 1: Introduction](https://openxr-tutorial.com/android/vulkan/1-introduction.html)
- **Graphics API**: Vulkan
- **Platform**: Android

## Key Concepts

- OpenXR ecosystem overview (Runtime, Loader, Layers)
- Project structure and build configuration
- Android platform initialization
- Common utility files for debugging and graphics abstraction

## Build & Deploy

```bash
# Build from repository root
gradlew :samples:openxr-tutorial:chapter1:assembleVulkanDebug

# Deploy to Quest 3
adb install -r app/build/outputs/apk/vulkanDebug/*.apk
```

## Differences from Original

This adaptation differs from the original tutorial in:
- Uses Maven for OpenXR loader dependency
- Uses CMake FetchContent for OpenXR SDK headers
- Configured for Quest 3 (Horizon OS v81, API 34)
- Integrated with repository-wide build system

See `INTEGRATION_NOTES.md` (when available) for detailed changes.

## Prerequisites

- Meta Quest 3 with developer mode enabled
- Android Studio with NDK r27
- See [Environment Setup](../../../docs/Environment_Setup.md)

## Next Chapter

After completing this chapter, proceed to Chapter 2 (coming soon).

---

**Tutorial Reference**: [openxr-tutorial.com/android/vulkan/1-introduction.html](https://openxr-tutorial.com/android/vulkan/1-introduction.html)
