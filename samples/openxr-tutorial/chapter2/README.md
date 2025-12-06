# Chapter 2: Setup

> OpenXR Tutorial Chapter 2 adapted for Meta Quest 3

## Overview

This chapter implements the core OpenXR infrastructure including instance creation, system identification, session management, and event handling. The application establishes a functional OpenXR session with Vulkan graphics binding but does not yet render frames (that's Chapter 3).

**Status**: 🔧 In Progress

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

```bash
# Build from chapter2 directory
gradlew assembleVulkanDebug

# Deploy to Quest 3
adb install -r app/build/outputs/apk/vulkanDebug/*.apk

# Or use test script
test_run.bat
```

## Expected Behavior

When running on Quest 3:
- Application launches and displays black screen
- OpenXR instance and session created successfully
- Event loop operates and responds to session state changes
- Application exits cleanly when user quits via Quest menu
- No rendering occurs yet (placeholder in main loop)

Check logcat for:
- OpenXR runtime name/version (Meta/Oculus runtime)
- System properties (Quest 3 hardware info)
- Session state transitions
- Event processing messages

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

This adaptation differs from the original tutorial in:
- Uses Maven for OpenXR loader dependency
- Uses CMake FetchContent for OpenXR SDK headers
- Configured for Quest 3 (Horizon OS v81, API 34)
- Integrated with repository-wide build system
- Vulkan-only (no D3D11/D3D12/OpenGL variants in this sample)

See `INTEGRATION_NOTES.md` for detailed changes.

## Prerequisites

- Meta Quest 3 with developer mode enabled
- Android Studio with NDK r27
- See [Environment Setup](../../../docs/Environment_Setup.md)

## Troubleshooting

### Application crashes on launch
- Check logcat for OpenXR error messages
- Verify Quest developer mode is enabled
- Ensure OpenXR runtime is available

### Session never reaches READY state
- Check Quest headset is being worn (proximity sensor)
- Verify permissions in AndroidManifest.xml
- Check logcat for session state events

### Build errors
- Verify NDK r27 is installed
- Check CMakeLists.txt paths to Common directory
- Ensure OpenXR loader AAR is resolved from Maven

## Next Chapter

After completing this chapter, proceed to **Chapter 3** (graphics rendering).

---

**Tutorial Reference**: [openxr-tutorial.com/android/vulkan/2-setup.html](https://openxr-tutorial.com/android/vulkan/2-setup.html)
