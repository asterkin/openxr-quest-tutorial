# Chapter 6

Quest/Android/Vulkan port of Chapter 6 from the OpenXR Tutorial series.

- Original tutorial: [Chapter 6: Next Steps](https://openxr-tutorial.com//android/vulkan/6-next-steps.html#nextsteps)

---

## Features Demonstrated

This chapter serves as a collection of advanced topics and next steps for optimizing an OpenXR application. The key features explored include:

-   **Multiview Rendering (`VK_KHR_multiview`)**: An advanced rendering technique that significantly reduces CPU overhead by allowing a single render pass to draw to both eye textures simultaneously. This is the primary feature implemented in this sample.
-   **OpenXR API Layers**: Explains how to use API layers for debugging and validation, such as `XR_APILAYER_LUNARG_core_validation`, which checks for correct API usage.
-   **Color Space Management**: Discusses the importance of correctly handling color spaces (e.g., linear vs. sRGB) within the rendering pipeline.
-   **Multithreaded Rendering**: Provides a conceptual overview of how to structure a multithreaded render loop by separating OpenXR frame submission calls (`xrWaitFrame`, `xrBeginFrame`, `xrEndFrame`) across different threads to improve performance.


