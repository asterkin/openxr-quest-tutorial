# OpenXR Hello XR Sample

> **For build, install, and run instructions, please refer to the main [OpenXR Samples Build Guidelines](../../Build_Guidelines.md).**
>
> This sample supports multiple graphics APIs. Use `gradlew installVulkanDebug` (recommended) or `gradlew installOpenGLESDebug`.

---

This is the canonical "hello world" sample from the Khronos OpenXR SDK, adapted for standalone builds on Quest. It demonstrates a complete rendering pipeline with controller input and stereo rendering.

## Purpose & Key Features

- Complete OpenXR session lifecycle management.
- Swapchain management and stereo rendering with view/projection matrices.
- Controller/hand input handling.
- An animated cube rendered in 3D space to verify the graphics pipeline.
- Based on the official [OpenXR-SDK-Source hello_xr](https://github.com/KhronosGroup/OpenXR-SDK-Source/tree/main/src/tests/hello_xr).

## Multi-API Architecture Note

Unlike other samples in this repository that are streamlined for a single graphics API (Vulkan), this sample intentionally retains its original multi-API architecture, supporting both **Vulkan** and **OpenGL ES**. This is to demonstrate OpenXR's graphics API-agnostic nature, a key feature of the cross-platform OpenXR standard.

The build system uses Gradle's "product flavors" to generate a separate APK for each graphics API.

| Flavor | Graphics API | App ID Suffix | Use Case |
|---|---|---|---|
| **Vulkan** | Vulkan 1.0 | `.vulkan` | Modern rendering, better performance. The recommended default for Quest. |
| **OpenGLES** | OpenGL ES 3.0 | `.opengles` | Demonstrates legacy API support. |

## Key Files

- **Application Logic**: [openxr_program.cpp](openxr_program.cpp) (OpenXR session lifecycle), [main.cpp](main.cpp) (Android entry point)
- **Graphics Backends**: [graphicsplugin_vulkan.cpp](graphicsplugin_vulkan.cpp), [graphicsplugin_opengles.cpp](graphicsplugin_opengles.cpp)
- **Build Configuration**: [build.gradle](build.gradle) (product flavors), [CMakeLists.txt](CMakeLists.txt) (native build)
- **Upstream Walkthrough**: [Application_Logic.md](Application_Logic.md) (a detailed code walkthrough from the original SDK)

---
For a detailed list of modifications made from the original upstream source, see [INTEGRATION_NOTES.md](INTEGRATION_NOTES.md).
