# OpenXR Tutorial - Chapter 3: GraphicsAPI & Swapchains

> **Graphics Pipeline Integration**: Vulkan rendering, swapchain management, and stereo projection

---

## Overview

Chapter 3 introduces the core graphics functionality for OpenXR applications:

- **Vulkan Graphics API**: Full Vulkan integration with instance, device, queue setup
- **Swapchain Management**: Creates and manages OpenXR swapchains for each eye
- **Stereo Rendering**: Per-view projection matrices and rendering pipeline
- **Shader Compilation**: GLSL to SPIR-V compilation integrated with CMake build system
- **Frame Loop**: Complete rendering loop with xrWaitFrame, xrBeginFrame, xrLocateViews, xrEndFrame

**Target Platform**: Meta Quest 3 (Vulkan 1.0.3+, OpenXR 1.0.34)

---

## What's New in Chapter 3

### GraphicsAPI Class (`GraphicsAPI_Vulkan.cpp`)
- Vulkan instance creation with required OpenXR extensions
- Physical device selection (Quest 3 GPU)
- Logical device and queue creation
- Swapchain image creation and management
- Basic cube rendering with vertex/fragment shaders
- Per-view projection matrix setup

### Shader System
- **Vertex Shader** (`VertexShader.glsl`): Transforms cube vertices with MVP matrices
- **Fragment Shader** (`PixelShader.glsl`): Per-view color coding (cyan for left eye, magenta for right eye)
- **Build Integration**: Shaders compiled to SPIR-V during CMake build, packaged in APK assets

### Rendering Features
- **Stereo cubes**: Two cubes rendered at different positions (floor level and table height)
- **Color coding**: Left eye sees cyan tint, right eye sees magenta tint
- **Depth testing**: Proper 3D depth perception
- **72Hz/90Hz rendering**: Smooth frame loop synchronized with Quest refresh rate

---

## Build Instructions

### Prerequisites
- Android SDK 34, NDK r27, Build-Tools 34.0.0
- Java 17+
- Meta Quest 3 with USB debugging enabled
- Working ADB connection to Quest

### Build for Quest 3 (Vulkan)

```batch
cd samples\openxr-tutorial\chapter3
gradlew assembleVulkanDebug
```

**Build output**: `app\build\outputs\apk\vulkan\debug\app-vulkan-debug.apk`

### Quick Test

```batch
test_run.bat
```

This script will:
1. Check Quest connection
2. Verify APK exists
3. Install APK to Quest
4. Launch application
5. Collect and display logs

### Cleanup

```batch
adb_cleanup.bat
```

Stops app, uninstalls, and clears logcat.

---

## Expected Behavior

### On Quest 3

When you put on the headset, you should see:

1. **Two colored cubes**:
   - One at floor level (y = 0.0)
   - One at table height (y = 1.0)
   - Both positioned 2 meters in front of you (z = -2.0)

2. **Stereo vision**:
   - Left eye sees cubes with cyan tint
   - Right eye sees cubes with magenta tint
   - Natural depth perception from stereo rendering

3. **Smooth rendering**:
   - 72Hz or 90Hz refresh rate (depending on Quest settings)
   - No judder or frame drops
   - Head tracking works (cubes stay fixed in world space)

### Expected Logs

```
OpenXR Tutorial Chapter 3
OpenXR Runtime: Oculus - 83.281.0
Reference Space Change pending for Session: 0x3
```

You may also see debug messenger logs from Vulkan validation layers.

---

## Troubleshooting

### Issue: Controller popup appears, app won't launch

**Symptom**: Quest shows "Connect your controllers" popup, app doesn't start.

**Logs**: `launch REJECTED (cancelPendingIntent)`, `app_launch_blocked_controller_required`

**Solution**: This was resolved by simplifying AndroidManifest.xml to match Chapter 1:
- Keep: `com.oculus.handtracking.frequency` and `com.oculus.handtracking.version` metadata
- Keep: `com.oculus.supportedDevices` metadata
- Remove: `com.oculus.vr.focusaware` metadata
- Remove: `com.oculus.handtracking.enabled` metadata
- Remove: `com.oculus.permission.HAND_TRACKING` permission

The manifest now mirrors Chapter 1/2 configuration.

### Issue: App crashes immediately (SIGSEGV)

**Symptom**: App launches but crashes with `Fatal signal 11 (SIGSEGV)`

**Logs**: Crash in `ReadBinaryFile()`, fault addr 0x0

**Cause**: Null AAssetManager pointer when loading shaders

**Solution**: The code now properly gets AAssetManager via JNI:
```cpp
JNIEnv *env;
androidApp->activity->vm->AttachCurrentThread(&env, nullptr);
jobject javaAssetManager = env->CallObjectMethod(androidApp->activity->clazz,
    env->GetMethodID(env->GetObjectClass(androidApp->activity->clazz),
    "getAssets", "()Landroid/content/res/AssetManager;"));
AAssetManager *assetManager = AAssetManager_fromJava(env, javaAssetManager);
```

### Issue: Shaders not found in APK

**Symptom**: Shaders compile but app can't load them

**Check**: Verify shaders are in APK:
```batch
"C:\Program Files\7-Zip\7z.exe" l app\build\outputs\apk\vulkan\debug\app-vulkan-debug.apk | findstr .spv
```

Expected output:
```
1364  assets/shaders/PixelShader.spv
2452  assets/shaders/VertexShader.spv
```

**Solution**: build.gradle must point to correct shader output directory:
```gradle
sourceSets {
    main {
        assets {
            srcDirs += [
                "$projectDir/.cxx/Debug/344h44l6/arm64-v8a",
                "$projectDir/.cxx/Release/344h44l6/arm64-v8a"
            ]
        }
    }
}
```

### Issue: No logs appearing in test_run.bat

**Symptom**: test_run.bat runs but shows no application logs

**Possible causes**:
1. App not launching (check for controller popup)
2. Native library not being loaded
3. Logcat buffer full or cleared too quickly

**Solutions**:
- Ensure app launches successfully (no controller popup)
- Wait longer after launch (test_run.bat waits 5 seconds)
- Clear logcat and re-run:
  ```batch
  adb logcat -c
  adb shell am start -n com.example.openxr_tutorial_ch3/android.app.NativeActivity
  timeout /t 5
  adb logcat -d -s openxr_tutorial:*
  ```

### Issue: Shader compilation fails

**Symptom**: Build fails with glslc errors

**Check**: glslc is in PATH and NDK is installed correctly

**Verify**:
```batch
where glslc
glslc --version
```

**Solution**: Ensure NDK r27 is installed and CMake can find glslc:
- Android Studio → SDK Manager → SDK Tools → NDK
- CMake searches for glslc in `$ENV{ANDROID_NDK_HOME}/shader-tools/*/glslc`

### Issue: Vulkan validation errors

**Symptom**: App runs but shows Vulkan validation layer warnings/errors

**Common causes**:
- Incorrect swapchain format
- Missing synchronization
- Invalid pipeline state

**Debug**: Check logcat for validation layer messages:
```batch
adb logcat -s openxr_tutorial:* DEBUG:* VK-*:*
```

---

## Differences from Original Khronos Tutorial

### Quest 3 Specific Adaptations

1. **Android Asset Loading**: Uses JNI to get AAssetManager (AndroidManifest doesn't provide direct access)

2. **Manifest Simplification**: Removed extra metadata that caused controller popup on Quest

3. **Shader Paths**: Android loads from `assets/shaders/`, uses AAssetManager API instead of fopen()

4. **Build System**:
   - Gradle integration for shader compilation
   - Custom sourceSets to package SPIR-V files
   - CMake dependency tracking for shaders

5. **Swapchain Format**: Uses Quest-preferred formats (likely VK_FORMAT_R8G8B8A8_SRGB)

6. **Testing Scripts**: Added test_run.bat and adb_cleanup.bat for rapid iteration

### Code Organization

- **Original**: Desktop-focused, uses platform-independent file I/O
- **This version**: Android-first, uses AAssetManager, packaged assets

---

## File Structure

```
chapter3/
├── README.md (this file)
├── main.cpp (OpenXR app entry point)
├── GraphicsAPI_Vulkan.cpp (Vulkan implementation)
├── GraphicsAPI.h (Graphics API interface)
├── VertexShader.glsl (GLSL vertex shader)
├── PixelShader.glsl (GLSL fragment shader)
├── CMakeLists.txt (native build configuration)
├── test_run.bat (automated test script)
├── adb_cleanup.bat (cleanup script)
└── app/
    ├── build.gradle (Android build config)
    └── src/main/
        └── AndroidManifest.xml (app manifest)
```

---

## Next Steps

- **Chapter 4**: Input handling (controllers, hand tracking)
- **Chapter 5**: Advanced rendering (textures, lighting, PBR)
- **Chapter 6**: Interaction (grab, teleport, UI)

---

## References

- [Khronos OpenXR Specification](https://registry.khronos.org/OpenXR/)
- [Meta Quest OpenXR Documentation](https://developer.oculus.com/documentation/native/android/mobile-openxr/)
- [Vulkan on Android](https://developer.android.com/ndk/guides/graphics/getting-started)

---

**Documentation Version**: 1.0
**Created**: 2025-12-07
**Target**: Meta Quest 3 (Horizon OS v81)
