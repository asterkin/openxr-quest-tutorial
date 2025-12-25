
# OpenXR Quest Tutorial Samples

> An index of progressive tutorial samples for learning OpenXR development on Meta Quest 3.

---

## Building

Build all OpenXR samples from this directory:

```bash
# Build all OpenXR samples - Debug
./gradlew.bat assembleAllDebug

# Build all OpenXR samples - Release
./gradlew.bat assembleAllRelease

# Clean all
./gradlew.bat cleanAll
```

Or build individual samples:

```bash
./gradlew.bat -p hello_world assembleDebug
./gradlew.bat -p hello_xr assembleVulkanDebug
./gradlew.bat -p tutorial assembleDebug
./gradlew.bat -p camera2_tutorial assembleDebug
```

> **Note**: The Gradle wrapper auto-downloads on first run.

For detailed build and troubleshooting instructions, see **[Build Guidelines](Build_Guidelines.md)**.

---

## Available Samples

| Sample | Complexity | Description |
|---|---|---|
| [hello_world - Minimal OpenXR Application](hello_world/) | ⭐ Beginner | Demonstrates basic Quest 3 setup with OpenXR session initialization and rendering loop. Focuses on instance/session creation, Vulkan binding, and basic rendering. |
| [hello_xr - Khronos OpenXR Conformance Sample](hello_xr/) | ⭐⭐ Intermediate | Adapted for Quest 3, this sample demonstrates standard OpenXR API usage including swapchain management, view configuration, input handling, and reference spaces. |
| [openxr-tutorial - Progressive OpenXR Tutorial Series](tutorial/) | ⭐⭐⭐ Advanced | A step-by-step tutorial series covering OpenXR instance/extension management, session lifecycle, event polling, Vulkan integration, swapchain, and frame rendering. |
| [camera2_tutorial - Camera2 + OpenXR Tutorial](camera2_tutorial/) | ⭐⭐⭐ Advanced | Standalone OpenXR + Vulkan sample with full-screen Camera2 (CPU YUV to RGBA) passthrough. |

---

## Learning Path

**Recommended progression**:

1. **Start**: [hello_world/](hello_world/) - Understand minimal OpenXR setup
2. **Foundation**: [hello_xr/](hello_xr/) - Learn standard OpenXR patterns
3. **Progressive**: [tutorial/](tutorial/) - Step-by-step tutorial series
   - Begin with Chapter 1 (setup)
   - Progress through Chapter 2 (session & events)
   - Continue with subsequent chapters
4. **Advanced**: [camera2_tutorial/](camera2_tutorial/) - Camera2 passthrough scaffold
5. **Advanced**: [meta/](meta/) - Explore Quest-specific features

Each sample builds on concepts from previous samples.
