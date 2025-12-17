
# OpenXR Quest Tutorial Samples

> An index of progressive tutorial samples for learning OpenXR development on Meta Quest 3.

---

## Build Guidelines

For detailed instructions on how to build, install, and troubleshoot the samples, refer to the **[OpenXR Samples Build Guidelines](Build_Guidelines.md)**.

---

## Available Samples

| Sample | Complexity | Description |
|---|---|---|
| [hello_world - Minimal OpenXR Application](hello_world/) | ⭐ Beginner | Demonstrates basic Quest 3 setup with OpenXR session initialization and rendering loop. Focuses on instance/session creation, Vulkan binding, and basic rendering. |
| [hello_xr - Khronos OpenXR Conformance Sample](hello_xr/) | ⭐⭐ Intermediate | Adapted for Quest 3, this sample demonstrates standard OpenXR API usage including swapchain management, view configuration, input handling, and reference spaces. |
| [openxr-tutorial - Progressive OpenXR Tutorial Series](openxr-tutorial/) | ⭐⭐⭐ Advanced | A step-by-step tutorial series covering OpenXR instance/extension management, session lifecycle, event polling, Vulkan integration, swapchain, and frame rendering. |
| [meta - Meta Quest Samples Collection](meta/) | ⭐⭐⭐ Advanced | A collection of Meta Quest-specific samples from the OVR SDK, showcasing features like rendering techniques, passthrough MR, hand tracking, and spatial anchors. |

---

## Learning Path

**Recommended progression**:

1. **Start**: [hello_world/](hello_world/) - Understand minimal OpenXR setup
2. **Foundation**: [hello_xr/](hello_xr/) - Learn standard OpenXR patterns
3. **Progressive**: [openxr-tutorial/](openxr-tutorial/) - Step-by-step tutorial series
   - Begin with Chapter 1 (setup)
   - Progress through Chapter 2 (session & events)
   - Continue with subsequent chapters
4. **Advanced**: [meta/](meta/) - Explore Quest-specific features

Each sample builds on concepts from previous samples.
