# OpenXR Tutorial (Android/Vulkan) — Quest Port

This directory contains a Quest-focused port of the **OpenXR Tutorial (Android + Vulkan)** series.

- Original tutorial: https://openxr-tutorial.com//android/vulkan/index.html
- Chapter projects: `Chapter1` … `Chapter6` (each is a standalone Android/Gradle + NDK project)
- Shared code: `Common/` (OpenXR + graphics helpers), `Shaders/` (used by later chapters)

### Chapters Overview

| Chapter | Description |
|---|---|
| [Chapter 1: Introduction & Setup](Chapter1/) | Basic project setup and OpenXR instance creation. |
| [Chapter 2: Instance/Session & Events](Chapter2/) | Managing OpenXR sessions and handling events. |
| [Chapter 3: Graphics (Vulkan)](Chapter3/) | Integrating the Vulkan graphics API. |
| [Chapter 4: Rendering](Chapter4/) | Setting up the rendering loop and swapchain. |
| [Chapter 5: Actions & Interaction](Chapter5/) | Handling user input and actions. (See `Chapter5/INTEGRATION_NOTE.md` for details.) |
| [Chapter 6: Multiview](Chapter6/) | Implementing multiview for stereoscopic rendering. |

