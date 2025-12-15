# OpenXR Hello World

> **For build, install, and run instructions, please refer to the main [OpenXR Samples Build Guidelines](../../Build_Guidelines.md).**

---

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

## Launch the Application 
For samples like `hello_world` that don't require immediate visual interaction, you can launch the application directly from your development machine without wearing the Quest headset. This is particularly useful for verifying logs.
To launch, use the Android Debug Bridge (ADB):
```powershell
adb shell am start -n com.example.openxr.helloworld/android.app.NativeActivity
```
The application will automatically exit after 5 seconds.

## Expected Log Output

When you run the sample and view the logs (`adb logcat -s "OpenXR_HelloWorld:*"`), you should see the following output:

```
OpenXR_HelloWorld: === OpenXR Hello World Starting ===
OpenXR_HelloWorld: ✅ OpenXR Instance created successfully!
OpenXR_HelloWorld: ✅ OpenXR System ID: 15992298772902969345
OpenXR_HelloWorld: Entering main loop (will auto-exit after 300 frames ~5 seconds)
...
OpenXR_HelloWorld: Auto-exiting after 300 frames
OpenXR_HelloWorld: OpenXR cleaned up
OpenXR_HelloWorld: === OpenXR Hello World Exiting ===
```

## Key Files

- [main.cpp](main.cpp) - Minimal OpenXR initialization code
- [AndroidManifest.xml](AndroidManifest.xml) - Quest metadata and permissions
- [CMakeLists.txt](CMakeLists.txt) - Native build configuration
- [build.gradle](build.gradle) - Android build configuration

