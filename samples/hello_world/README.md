# OpenXR Hello World

Minimal OpenXR sample for Meta Quest demonstrating the simplest possible OpenXR application.

## Purpose

This sample verifies your OpenXR development environment by:
- Initializing the OpenXR loader on Android
- Creating an OpenXR instance
- Detecting the Quest headset (System ID)
- Running a basic event loop

**No rendering** - this is purely for testing OpenXR SDK integration.

## Architecture

**Build Approach**: Maven + CMake FetchContent (no manual SDK installation required)

- **OpenXR Loader**: `org.khronos.openxr:openxr_loader_for_android:1.1.54` (Maven Central)
- **OpenXR Headers**: Downloaded automatically via CMake FetchContent from OpenXR-SDK-Source
- **Build System**: Gradle 8.13 + CMake 3.22.1 + Android NDK r27

## Prerequisites

- Android SDK with NDK r27
- Meta Quest 3 with USB debugging enabled
- ADB in PATH

## Build Commands

```bash
# Build APK
gradlew assembleDebug

# Or use wrapper on Unix
./gradlew assembleDebug
```

**Build Output**: `build/outputs/apk/debug/hello_world-debug.apk` (~8.4 MB)

## Testing

### Quick Test (Automated)
```bash
test_run.bat
```

This will:
1. Check Quest connection
2. Verify APK exists
3. Install APK
4. Launch app (auto-exits after ~5 seconds)
5. Collect and display logs

### Manual Testing
```bash
# Install
adb install -r build/outputs/apk/debug/hello_world-debug.apk

# Launch
adb shell am start -n com.example.openxr.helloworld/android.app.NativeActivity

# View logs
adb logcat -s "OpenXR_HelloWorld:*"
```

### Expected Log Output

```
OpenXR_HelloWorld: === OpenXR Hello World Starting ===
OpenXR_HelloWorld: ✅ OpenXR Instance created successfully!
OpenXR_HelloWorld: ✅ OpenXR System ID: 15992298772902969345
OpenXR_HelloWorld: Entering main loop (will auto-exit after 300 frames ~5 seconds)
OpenXR_HelloWorld: APP_CMD_START
OpenXR_HelloWorld: Frame 60 - Still running...
...
OpenXR_HelloWorld: Auto-exiting after 300 frames
OpenXR_HelloWorld: OpenXR cleaned up
OpenXR_HelloWorld: === OpenXR Hello World Exiting ===
```

## Cleanup

```bash
# Uninstall app
adb uninstall com.example.openxr.helloworld
```

## Key Files

- [main.cpp](main.cpp) - Minimal OpenXR initialization code
- [AndroidManifest.xml](AndroidManifest.xml) - Quest metadata and permissions
- [CMakeLists.txt](CMakeLists.txt) - Native build configuration
- [build.gradle](build.gradle) - Android build configuration
- [test_run.bat](test_run.bat) - Automated test script

## Quest-Specific Configuration

The AndroidManifest includes hand tracking metadata to bypass controller requirements:

```xml
<meta-data android:name="com.oculus.handtracking.frequency" android:value="HIGH" />
<meta-data android:name="com.oculus.handtracking.version" android:value="V2.0" />
<meta-data android:name="com.oculus.supportedDevices" android:value="quest3|quest2|questpro" />
```

Without this, Quest will block app launch until controllers are detected.

## Troubleshooting

**App doesn't launch**: Pick up Quest controllers (system requires them initially)

**No logs appear**: Check log tag is correct: `adb logcat -s "OpenXR_HelloWorld:*"`

**Build fails**: Verify NDK r27 is installed and `ANDROID_NDK_ROOT` is set

**App hangs**: Force stop with `adb shell am force-stop com.example.openxr.helloworld`

## Next Steps

After verifying this sample works:
1. Explore `samples/hello_xr/` for full rendering pipeline
2. Add swapchain and rendering to this sample
3. Implement interaction (controllers, hands)

## References

- Based on [openxr-tutorial.com](https://openxr-tutorial.com/android/vulkan/1-introduction.html)
- [OpenXR Specification 1.1](https://registry.khronos.org/OpenXR/specs/1.1/html/xrspec.html)
- [OpenXR-SDK-Source](https://github.com/KhronosGroup/OpenXR-SDK-Source)
