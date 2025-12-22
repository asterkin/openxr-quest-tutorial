# ADR-0012: Graphics API Strategy - Vulkan for OpenXR Tutorials, OpenGL-ES for Meta Samples

**Status:** Accepted

**Date:** 2025-12-22

## Context

This repository contains two categories of samples:

1. **OpenXR tutorials** (`openxr/tutorial/`) - Custom-built samples teaching OpenXR fundamentals
2. **Meta XR samples** (`meta/Samples/`) - Samples from Meta's ovr_openxr_mobile_sdk demonstrating Meta-specific extensions

The Meta samples were imported with their original OpenGL-ES rendering backend. Converting them to Vulkan would require significant effort with minimal educational benefit.

### Vulkan vs OpenGL-ES: Brief Comparison

| Aspect | Vulkan | OpenGL-ES |
|--------|--------|-----------|
| **API Level** | Low-level, explicit control | Higher-level, driver-managed |
| **CPU Overhead** | Lower (multi-threaded command buffers) | Higher (single-threaded, driver overhead) |
| **Complexity** | Higher (verbose initialization, manual synchronization) | Lower (simpler API surface) |
| **Quest 3 Support** | Native, optimal performance path | Well-supported, mature ecosystem |
| **Learning Curve** | Steeper | Gentler |

Both APIs are fully supported on Quest 3. Vulkan offers better performance potential for production applications, while OpenGL-ES provides faster iteration for learning and prototyping.

### OpenXR Graphics API Transparency

OpenXR does **not** provide abstraction between graphics APIs. Switching from OpenGL-ES to Vulkan (or vice versa) requires **source code changes**, not just build configuration:

| Component | Vulkan | OpenGL-ES |
|-----------|--------|-----------|
| **Extension** | `XR_KHR_vulkan_enable` / `_enable2` | `XR_KHR_opengl_es_enable` |
| **Graphics Binding** | `XrGraphicsBindingVulkanKHR` | `XrGraphicsBindingOpenGLESAndroidKHR` |
| **Swapchain Images** | `XrSwapchainImageVulkanKHR` (VkImage) | `XrSwapchainImageOpenGLESKHR` (GLuint) |
| **Requirements Query** | `xrGetVulkanGraphicsRequirementsKHR` | `xrGetOpenGLESGraphicsRequirementsKHR` |

In practice, supporting both APIs in a single codebase requires:
- Conditional compilation or runtime branching for graphics initialization
- Separate rendering backends (Vulkan command buffers vs GL draw calls)
- Different resource management patterns (explicit vs driver-managed)

This reinforces why maintaining separate sample sets (Vulkan tutorials, OpenGL-ES Meta samples) is more practical than attempting a unified abstraction layer.

## Decision

We will maintain **different graphics APIs** for different sample categories:

- **OpenXR tutorials**: Use **Vulkan** exclusively
- **Meta XR samples**: Keep **OpenGL-ES** as imported

This allows us to:
1. Focus Vulkan expertise in our custom tutorials
2. Preserve Meta samples as-is for compatibility with Meta's documentation
3. Speed up the overall build by avoiding unnecessary graphics API conversions

## Consequences

### Positive
- Faster build times (no need to port Meta samples to Vulkan)
- Meta samples remain consistent with Meta's official documentation
- Clear separation: Vulkan learning happens in tutorials, Meta extension learning in samples
- Reduced maintenance burden

### Negative
- Developers must understand both APIs to work with all samples
- No unified rendering abstraction across the repository
- Meta samples won't benefit from Vulkan-specific optimizations

### Neutral
- Each sample category has its own graphics initialization patterns
- Documentation must specify which API each sample uses

## References
- [ADR-0008: Integrate Meta OpenXR Samples with Minimal Changes](adr-0008-integrate-meta-openxr-samples-with-minimal-changes.md)
- [Vulkan vs OpenGL-ES on Quest](https://developer.oculus.com/documentation/native/android/mobile-graphics-overview/)
- [OpenXR Graphics Bindings](https://registry.khronos.org/OpenXR/specs/1.0/html/xrspec.html#XR_KHR_vulkan_enable)
