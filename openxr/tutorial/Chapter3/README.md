# Chapter 3

Quest/Android/Vulkan port of Chapter 3 from the OpenXR Tutorial series.

- Original tutorial: [Chapter 3: Graphics](https://openxr-tutorial.com//android/vulkan/3-graphics.html#graphics)

---

## Features Demonstrated

This chapter integrates a graphics API (Vulkan) to enable rendering within the OpenXR application. Key features include:

-   **Swapchain Management**:
    -   Enumerating supported swapchain formats.
    -   Creating and destroying `XrSwapchain` handles.
    -   Retrieving and managing swapchain images.
-   **Render Loop Implementation**:
    -   Defining a reference space (`XrSpace`) to establish a world coordinate system.
    -   Implementing the core frame loop (`xrBeginFrame`, `xrEndFrame`).
    -   Acquiring swapchain images for rendering.
    -   Locating views to get projection and view matrices for each eye.
-   **Graphics API Integration**: Demonstrates how to bind an OpenXR session with a Vulkan rendering backend to display content.

