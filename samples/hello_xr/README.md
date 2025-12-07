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

**Build Approach**: Self-contained with CMake FetchContent (no manual SDK installation required)

- **OpenXR Loader**: Built from source via CMake FetchContent from OpenXR-SDK-Source
- **OpenXR Headers**: Downloaded automatically via CMake FetchContent
- **Build System**: Gradle 8.13 + CMake 3.22.1 + Android NDK r27
- **Graphics**: Vulkan (default) or OpenGL ES variants
- **GLAD Loader**: GLAD2 for OpenGL/OpenGL ES function loading

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

### Automated Cleanup
```bash
adb_cleanup.bat
```

This script will:
1. Clear logcat buffer
2. Force-stop running app
3. Uninstall both Vulkan and OpenGL ES variants
4. Clear app data/cache

### Manual Cleanup
```bash
# Uninstall Vulkan variant
adb uninstall org.khronos.openxr.hello_xr.vulkan

# Uninstall OpenGL ES variant
adb uninstall org.khronos.openxr.hello_xr.opengles
```

## Key Files

### Source Code
- [main.cpp](main.cpp) - Application entry point (NativeActivity integration)
- [openxr_program.cpp](openxr_program.cpp) - OpenXR session lifecycle and rendering loop
- [graphicsplugin_vulkan.cpp](graphicsplugin_vulkan.cpp) - Vulkan rendering backend
- [graphicsplugin_opengles.cpp](graphicsplugin_opengles.cpp) - OpenGL ES rendering backend
- [common/gfxwrapper_opengl.c](common/gfxwrapper_opengl.c) - OpenGL window/context wrapper

### Build Configuration
- [AndroidManifest.xml](AndroidManifest.xml) - Quest metadata, permissions, hand tracking
- [CMakeLists.txt](CMakeLists.txt) - Native build with FetchContent for OpenXR SDK
- [build.gradle](build.gradle) - Android build with Vulkan/OpenGLES product flavors

### Utilities
- [adb_cleanup.bat](adb_cleanup.bat) - Device cleanup script
- [test_run.bat](test_run.bat) - Automated build-install-run-log workflow

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

### Upstream Documentation (from OpenXR-SDK-Source)
- [Application_Logic.md](Application_Logic.md) - Complete walkthrough of hello_xr code flow
- [Developers_Guidelines.md](Developers_Guidelines.md) - Development best practices for OpenXR
- [Developers_Guidelines_Windows.md](Developers_Guidelines_Windows.md) - Windows-specific development setup

### Tutorial Repository Documentation
- See repository root [docs/](../../docs/) for environment setup and architecture guides

## Differences from Upstream OpenXR-SDK-Source

This sample is adapted from the canonical [OpenXR-SDK-Source hello_xr](https://github.com/KhronosGroup/OpenXR-SDK-Source/tree/main/src/tests/hello_xr) with these modifications:

**For detailed implementation changes, see [INTEGRATION_NOTES.md](INTEGRATION_NOTES.md)**

### Build System Changes
1. ✅ **Self-contained CMake**: Uses FetchContent to download OpenXR SDK instead of parent project
2. ✅ **Standalone loader build**: Builds `openxr_loader` from source (no Maven AAR dependency)
3. ✅ **GLAD2 integration**: Includes GLAD2 for OpenGL ES function loading
4. ✅ **gfxwrapper_opengl.c**: Added missing OpenGL wrapper functions to CMake build

### Tooling Updates
5. ✅ **NDK r27**: Updated from NDK r23 for Quest 3 / Horizon OS v81 compatibility
6. ✅ **Gradle 8.13**: Updated from Gradle 7.x with AGP 8.13.0
7. ✅ **CMake 3.22.1**: Pinned CMake version for reproducible builds

### Quest 3 Optimizations
8. ✅ **Quest metadata**: Added `com.oculus.handtracking` and `com.oculus.supportedDevices` to manifest
9. ✅ **ANativeActivity linker flag**: Added `-u ANativeActivity_onCreate` to fix runtime loading
10. ✅ **GLAD_GLES2 define**: Enabled OpenGL ES 2.0+ support in GLAD loader

### Development Experience
11. ✅ **Automated testing**: [test_run.bat](test_run.bat) for build-install-run-log workflow
12. ✅ **Device cleanup**: [adb_cleanup.bat](adb_cleanup.bat) for complete app removal

### Maintained from Upstream
- ✅ **Source code**: Identical to upstream (main.cpp, openxr_program.cpp, graphics plugins)
- ✅ **Documentation**: Upstream docs copied as-is ([Application_Logic.md](Application_Logic.md), etc.)
- ✅ **Controller bindings**: Full support for Oculus Touch, Vive, Index, WMR controllers

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
