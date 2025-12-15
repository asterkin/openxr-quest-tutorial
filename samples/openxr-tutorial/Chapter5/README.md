# Chapter 5

Quest/Android/Vulkan port of Chapter 5 from the OpenXR Tutorial series.

- Original tutorial:  [Chapter 5: Extensions](https://openxr-tutorial.com//android/vulkan/5-extensions.html#extensions)
- [Integration notes (Quest-specific)](./INTEGRATION_NOTE.md)

---

## Features Demonstrated

This chapter explains how to query for and enable OpenXR extensions, demonstrating two specific features:

-   **`XR_EXT_hand_tracking`**: Enables the use of hand tracking as an alternative to controllers. This includes retrieving the positions and orientations of hand joints.
-   **`XR_KHR_composition_layer_depth`**: Shows how to submit depth information alongside the color swapchain. This is crucial for proper occlusion and depth testing in Augmented Reality (AR) and Mixed Reality (MR) applications, allowing virtual objects to correctly appear in front of or behind real-world objects.


