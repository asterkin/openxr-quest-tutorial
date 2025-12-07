# Chapter 2: Setup

> OpenXR Tutorial Chapter 2 adapted for Meta Quest 3

## Overview

This chapter implements the core OpenXR infrastructure including instance creation, system identification, session management, and event handling. The application establishes a functional OpenXR session with Vulkan graphics binding but does not yet render frames (that's Chapter 3).

**Status**: ✅ Complete

## Original Tutorial

This sample is adapted from:
- **Source**: [openxr-tutorial.com - Chapter 2: Setup](https://openxr-tutorial.com/android/vulkan/2-setup.html)
- **Graphics API**: Vulkan
- **Platform**: Android

## Key Concepts

### OpenXR Instance
- Creating an XrInstance with application/engine information
- Enumerating and validating API layers and extensions
- Debug messenger setup via `XR_EXT_debug_utils`

### System Identification
- Getting XrSystemId for head-mounted display form factor
- Querying system properties (hardware capabilities)

### Session Management
- Creating XrSession with Vulkan graphics binding
- Handling session lifecycle (begin/end)
- Session state machine transitions

### Event Polling
- Processing OpenXR events in main loop
- Handling session state changes:
  - READY → begin session
  - STOPPING → end session
  - EXITING/LOSS_PENDING → shutdown application
- Responding to instance loss and interaction profile changes

## Build & Deploy

### Quick Test (Recommended)

```bash
# From chapter2 directory
.\test_run.bat
```

This automated script:
1. Checks for connected Quest device
2. Verifies APK exists
3. Installs APK via ADB
4. Launches application
5. Collects and displays logs

### Manual Build & Deploy

```bash
# Build from repository root
gradlew :samples:openxr-tutorial:chapter2:assembleVulkanDebug

# Or from chapter2 directory
..\..\..\gradlew.bat assembleVulkanDebug

# Deploy to Quest 3
adb install -r app\build\outputs\apk\vulkan\debug\app-vulkan-debug.apk

# Launch
adb shell am start -n com.example.openxr_tutorial_ch2/android.app.NativeActivity

# View logs
adb logcat -s openxr_tutorial:*
```

### Cleanup

```bash
# Uninstall and clear logs
.\adb_cleanup.bat
```

## Expected Behavior

When running on Quest 3:
- Application launches and displays black screen
- OpenXR instance and session created successfully
- Event loop operates and responds to session state changes
- Application exits cleanly when user quits via Quest menu
- No rendering occurs yet (placeholder in main loop)

### Expected Logs

```
openxr_tutorial: redirected cout
openxr_tutorial: Testing cout redirect.
openxr_tutorial: OpenXR Tutorial Chapter 2
openxr_tutorial: OpenXR Runtime: Oculus - 83.281.0
openxr_tutorial: xrCreateDebugUtilsMessengerEXT(VERBOSE / GEN): ...
openxr_tutorial: OPENXR: Reference Space Change pending for Session: 0x3
```

Key indicators:
- **Runtime**: Oculus 83.281.0 (or newer)
- **Session ID**: e.g., `Session: 0x3`
- **Debug messenger**: Loader trampoline/terminator messages
- **Events**: Reference space changes indicate tracking is active

## Implementation Details

### New Methods (vs Chapter 1)

- `CreateInstance()` - Instance creation with extension validation
- `DestroyInstance()` - Instance cleanup
- `GetInstanceProperties()` - Log runtime information
- `CreateDebugMessenger()` - Setup debug callbacks
- `DestroyDebugMessenger()` - Cleanup debug messenger
- `GetSystemID()` - Obtain system ID and properties
- `CreateSession()` - Create session with Vulkan binding
- `DestroySession()` - Session cleanup
- `PollEvents()` - Event loop processing

### Member Variables

```cpp
XrInstance m_xrInstance;
XrDebugUtilsMessengerEXT m_debugUtilsMessenger;
XrSystemId m_systemID;
XrSystemProperties m_systemProperties;
XrSession m_session;
XrSessionState m_sessionState;
std::unique_ptr<GraphicsAPI> m_graphicsAPI;
bool m_applicationRunning;
bool m_sessionRunning;
```

## Differences from Original

This Quest 3 adaptation includes:

### Build System
- Maven dependency for OpenXR loader (`org.khronos.openxr:openxr_loader_for_android:1.1.54`)
- CMake FetchContent for OpenXR SDK headers (release-1.1.54)
- Android Gradle Plugin configuration for NativeActivity
- Prefab support for native library integration

### Platform Configuration
- Target: Horizon OS v81 (Android API 34)
- Vulkan 1.0.3+ requirement
- Quest-specific metadata (`com.oculus.supportedDevices`)
- Hand tracking metadata (HIGH frequency, V2.0)
- **Focus-aware flag** (`com.oculus.vr.focusaware`) to bypass controller requirement dialog

### Code Adaptations
- Android logging via `__android_log_write` with `openxr_tutorial` tag
- Stream redirection for `std::cout`/`std::cerr`
- Fixed `VK_MAKE_API_VERSION` macro guard (was causing redefinition warning)
- Vulkan-only (no D3D11/D3D12/OpenGL variants in this sample)

See `INTEGRATION_NOTES.md` for detailed implementation changes.

## Prerequisites

- Meta Quest 3 with developer mode enabled
- Android Studio with NDK r27
- See [Environment Setup](../../../docs/Environment_Setup.md)

## Troubleshooting

### No logs appearing
- Ensure native library name matches in `AndroidManifest.xml` (`openxr_tutorial_ch2`)
- Check CMakeLists.txt library name matches
- Verify APK contains `libopenxr_tutorial_ch2.so`:
  ```bash
  unzip -l app\build\outputs\apk\vulkan\debug\app-vulkan-debug.apk | grep libopenxr
  ```

### Controller warning dialog appears
- The app includes `com.oculus.vr.focusaware` metadata to bypass this
- If dialog still appears, dismiss it manually - Chapter 2 doesn't use input
- Alternatively, connect controllers before launching

### Application crashes on launch
- Check `adb logcat` for native crashes or OpenXR errors
- Verify Quest developer mode is enabled
- Ensure OpenXR runtime is available (Quest system software up to date)

### Session never reaches READY state
- Check Quest headset is being worn (proximity sensor active)
- Verify permissions in AndroidManifest.xml
- Check logcat for session state transition events

### Build errors
- Verify NDK r27 is installed
- Check CMakeLists.txt paths to Common directory (symlink valid)
- Ensure OpenXR loader AAR is resolved from Maven Central
- Run `gradlew --refresh-dependencies` if dependency resolution fails

## Next Chapter

After completing this chapter, proceed to **Chapter 3** (graphics rendering).

---

**Tutorial Reference**: [openxr-tutorial.com/android/vulkan/2-setup.html](https://openxr-tutorial.com/android/vulkan/2-setup.html)
