# OpenXR Hello XR Sample

Full-featured OpenXR sample for Meta Quest demonstrating complete rendering pipeline with Vulkan or OpenGL ES.

## Purpose

This is the canonical "hello world" sample from the Khronos OpenXR SDK, adapted for standalone builds:
- Complete OpenXR session lifecycle
- Vulkan or OpenGL ES rendering
- Swapchain management
- Controller/hand input
- Stereo rendering with view/projection matrices
- Animated cube rendering in 3D space

**Based on**: [OpenXR-SDK-Source hello_xr](https://github.com/KhronosGroup/OpenXR-SDK-Source/tree/main/src/tests/hello_xr)

## Architecture

**Build Approach**: Maven + CMake FetchContent (no manual SDK installation required)

- **OpenXR Loader**: `org.khronos.openxr:openxr_loader_for_android:1.1.54` (Maven Central)
- **OpenXR Headers**: Downloaded automatically via CMake FetchContent from OpenXR-SDK-Source
- **Build System**: Gradle 8.13 + CMake 3.22.1 + Android NDK r27
- **Graphics**: Vulkan (default) or OpenGL ES variants

## Prerequisites

- Android SDK with NDK r27
- Meta Quest 3 with USB debugging enabled
- ADB in PATH

## Build Commands

```bash
# Build Vulkan variant (default)
gradlew assembleVulkanDebug

# Or OpenGL ES variant
gradlew assembleOpenGLESDebug

# Or build both
gradlew assembleDebug
```

**Build Outputs**:
- Vulkan: `build/outputs/apk/Vulkan/debug/hello_xr-Vulkan-debug.apk`
- OpenGL ES: `build/outputs/apk/OpenGLES/debug/hello_xr-OpenGLES-debug.apk`

## Testing

### Quick Test (Automated)
```bash
test_run.bat
```

This will:
1. Check Quest connection
2. Verify APK exists
3. Install APK (Vulkan variant)
4. Launch app
5. Stream logs in real-time

### Manual Testing
```bash
# Install Vulkan variant
adb install -r build/outputs/apk/Vulkan/debug/hello_xr-Vulkan-debug.apk

# Launch
adb shell am start -n org.khronos.openxr.hello_xr.vulkan/android.app.NativeActivity

# View logs
adb logcat -s "hello_xr:*"
```

### Expected Behavior

**Put on headset** - you should see:
- Two animated cubes rotating in 3D space
- Stereo rendering (different view for each eye)
- Smooth 72 FPS rendering
- Controller tracking (if controllers active)

**Logs should show**:
```
hello_xr: CreateInstance
hello_xr: Using OpenXR runtime: Oculus OpenXR Mobile
hello_xr: CreateSession
hello_xr: Swapchain format: ...
hello_xr: Entering session running state
hello_xr: Frame submitted
...
```

## Cleanup

```bash
# Uninstall Vulkan variant
adb uninstall org.khronos.openxr.hello_xr.vulkan

# Uninstall OpenGL ES variant
adb uninstall org.khronos.openxr.hello_xr.opengles
```

## Key Files

- [main.cpp](main.cpp) - Application entry point
- [openxr_program.cpp](openxr_program.cpp) - OpenXR session management
- [graphicsplugin_vulkan.cpp](graphicsplugin_vulkan.cpp) - Vulkan rendering
- [graphicsplugin_opengles.cpp](graphicsplugin_opengles.cpp) - OpenGL ES rendering
- [AndroidManifest.xml](AndroidManifest.xml) - Quest metadata and permissions
- [CMakeLists.txt](CMakeLists.txt) - Native build configuration
- [build.gradle](build.gradle) - Android build with Vulkan/OpenGLES flavors

## Product Flavors

The build supports two graphics API flavors:

| Flavor | Graphics API | App ID Suffix | Use Case |
|--------|--------------|---------------|----------|
| **Vulkan** | Vulkan 1.0 | `.vulkan` | Modern rendering, better performance |
| **OpenGLES** | OpenGL ES 3.0 | `.opengles` | Legacy devices, broader compatibility |

Both variants can be installed simultaneously on the same device.

## Troubleshooting

**Black screen in headset**: Check logcat for errors, ensure controllers are connected

**Build fails**: Verify NDK r27 is installed: `gradlew --stop && gradlew clean`

**App crashes on launch**: Check OpenXR runtime version is up to date on Quest

**Gradle sync fails**: Delete `.gradle` and `.cxx` folders, resync

## Documentation

**Detailed Analysis**:
- [Application_Logic.md](Application_Logic.md) - Complete code walkthrough
- [Developers_Guidelines.md](Developers_Guidelines.md) - Development best practices
- [OpenXR_Tutorial.md](OpenXR_Tutorial.md) - Learning guide

## Differences from Original

This version differs from the upstream OpenXR-SDK-Source hello_xr:

1. ✅ **Self-contained build**: Uses Maven + FetchContent instead of parent CMake project
2. ✅ **NDK r27**: Updated from NDK r23 for Quest 3 compatibility
3. ✅ **Gradle 8.13**: Updated from Gradle 7.x
4. ✅ **Simplified CMake**: Removed AAR extraction, uses FetchContent for headers
5. ✅ **Quest-optimized**: AndroidManifest tuned for Quest 3
6. ✅ **Test automation**: Added test_run.bat script

## Next Steps

After running this sample successfully:
1. Study [Application_Logic.md](Application_Logic.md) to understand OpenXR flow
2. Modify rendering code to experiment with different visuals
3. Add interaction with controllers/hands
4. Explore Meta-specific extensions in `samples/meta/` directory

## References

- [OpenXR Specification 1.1](https://registry.khronos.org/OpenXR/specs/1.1/html/xrspec.html)
- [OpenXR-SDK-Source](https://github.com/KhronosGroup/OpenXR-SDK-Source)
- [OpenXR Tutorial](https://openxr-tutorial.com/)
