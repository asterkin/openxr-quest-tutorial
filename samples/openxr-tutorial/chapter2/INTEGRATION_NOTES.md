# Chapter 2 Integration Notes

> Detailed adaptation notes for Quest 3 implementation

## Overview

This document details the changes made to adapt the original OpenXR Tutorial Chapter 2 for Meta Quest 3 with our repository's build system.

**Original Source**: [OpenXR-Tutorials v1.0.13 - Chapter2.zip](https://github.com/KhronosGroup/OpenXR-Tutorials/releases/tag/v1.0.13)

**Reference**: `C:\Users\PRO\Projects\openxr-quest-tutorial\temp_chapter2\Chapter2\` (removed after implementation)

## Build System Changes

### 1. OpenXR SDK Integration

**Original Approach**:
- Downloaded OpenXR SDK manually
- Headers and loader library bundled with project

**Our Approach**:
```cmake
# CMakeLists.txt
FetchContent_Declare(
    openxr
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenXR-SDK-Source.git
    GIT_TAG release-1.1.54
)
```

```gradle
// app/build.gradle
dependencies {
    implementation 'org.khronos.openxr:openxr_loader_for_android:1.1.54'
}

android.buildFeatures.prefab = true
```

**Rationale**: Decouples SDK from repository, easier updates, standard Android dependency management

### 2. Native Library Configuration

**File**: `app/build.gradle`

```gradle
android {
    namespace 'com.example.openxr_tutorial_ch2'

    defaultConfig {
        applicationId "com.example.openxr_tutorial_ch2"

        externalNativeBuild {
            cmake {
                arguments "-DANDROID_STL=c++_static",
                          "-DANDROID_ARM_NEON=TRUE",
                          "-DXR_USE_PLATFORM_ANDROID=ON",
                          "-DXR_USE_GRAPHICS_API_VULKAN=ON"
            }
        }

        ndk {
            abiFilters 'arm64-v8a'  // Quest 3 is 64-bit only
        }
    }

    packagingOptions {
        jniLibs.useLegacyPackaging = true  // Required for NativeActivity
    }
}
```

**Changes from Original**:
- Explicit package name for isolation
- CMake arguments define platform macros
- Single ABI (arm64-v8a) - Quest 3 doesn't need 32-bit
- Legacy packaging for NativeActivity compatibility

### 3. CMake Build Configuration

**File**: `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.22.1)
project(openxr_tutorial_ch2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch OpenXR SDK headers
include(FetchContent)
FetchContent_Declare(
    openxr
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenXR-SDK-Source.git
    GIT_TAG release-1.1.54
)
FetchContent_MakeAvailable(openxr)

# Main application library
add_library(openxr_tutorial_ch2 SHARED
    ../main.cpp
    ../Common/GraphicsAPI_Vulkan.cpp
    ../Common/OpenXRDebugUtils.cpp
)

target_include_directories(openxr_tutorial_ch2 PRIVATE
    ${openxr_SOURCE_DIR}/include
    ${openxr_SOURCE_DIR}/src/common
    ../Common
)

target_link_libraries(openxr_tutorial_ch2
    openxr::openxr_loader  # From Prefab
    android
    log
    vulkan
)

target_compile_definitions(openxr_tutorial_ch2 PRIVATE
    XR_USE_PLATFORM_ANDROID
    XR_USE_GRAPHICS_API_VULKAN
)
```

**Key Differences**:
- FetchContent for headers (not entire SDK build)
- Links against Maven-provided `openxr_loader` via Prefab
- Explicit Vulkan linkage (Quest 3 primary API)
- Common files referenced via relative paths (symlink)

## Platform-Specific Adaptations

### 1. AndroidManifest.xml

**File**: `app/src/main/AndroidManifest.xml`

```xml
<manifest xmlns:android="http://schemas.android.com/apk/res/android">
    <!-- Vulkan 1.0.3 requirement (Quest 3 supports Vulkan 1.3) -->
    <uses-feature
        android:name="android.hardware.vulkan.version"
        android:version="0x00400003"
        android:required="true" />

    <!-- Quest 3 6DOF tracking -->
    <uses-feature
        android:name="android.hardware.vr.headtracking"
        android:version="1"
        android:required="true" />

    <application android:hasCode="false">
        <!-- Hand tracking metadata -->
        <meta-data android:name="com.oculus.handtracking.frequency" android:value="HIGH" />
        <meta-data android:name="com.oculus.handtracking.version" android:value="V2.0" />

        <!-- Supported devices -->
        <meta-data android:name="com.oculus.supportedDevices"
                   android:value="quest3|quest2|questpro" />

        <!-- IMPORTANT: Bypass controller requirement dialog -->
        <meta-data android:name="com.oculus.vr.focusaware" android:value="true" />

        <activity android:name="android.app.NativeActivity">
            <meta-data android:name="android.app.lib_name"
                       android:value="openxr_tutorial_ch2" />

            <!-- OpenXR launcher intents -->
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
                <category android:name="org.khronos.openxr.intent.category.IMMERSIVE_HMD" />
            </intent-filter>

            <!-- Quest-specific intent -->
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
                <category android:name="com.oculus.intent.category.VR" />
            </intent-filter>
        </activity>
    </application>
</manifest>
```

**Critical Addition**: `com.oculus.vr.focusaware` metadata
- **Problem**: Quest shows controller requirement dialog on launch
- **Solution**: This flag tells Quest the app can launch without controllers
- **Reason**: Chapter 2 doesn't use input - only establishes session

### 2. Android Logging

**File**: `Common/DebugOutput.h` (lines 177-197)

```cpp
#ifdef __ANDROID__
#include <android/log.h>
#include <sstream>

#define XR_TUT_LOG_TAG "openxr_tutorial"
#define XR_TUT_LOG(...) {                                                           \
        std::ostringstream ostr;                                                    \
        ostr<<__VA_ARGS__;                                                          \
        __android_log_write(ANDROID_LOG_DEBUG, XR_TUT_LOG_TAG, ostr.str().c_str()); \
    }
#define XR_TUT_LOG_ERROR(...) {                                                     \
        std::ostringstream ostr;                                                    \
        ostr<<__VA_ARGS__;                                                          \
        __android_log_write(ANDROID_LOG_ERROR, XR_TUT_LOG_TAG, ostr.str().c_str()); \
    }
#else
#include <iostream>
#define XR_TUT_LOG(...) std::cout << __VA_ARGS__ << "\n"
#define XR_TUT_LOG_ERROR(...) std::cerr << __VA_ARGS__ << "\n"
#endif
```

**Key Points**:
- Unified log tag: `"openxr_tutorial"` (same as Chapter 1)
- Macros support stream operators: `XR_TUT_LOG("Value: " << value)`
- Priority levels: DEBUG for info, ERROR for errors
- Visible via `adb logcat -s openxr_tutorial:*`

### 3. Vulkan Compatibility Fix

**File**: `Common/GraphicsAPI_Vulkan.cpp` (lines 23-25)

**Issue**: Macro redefinition warning
```
warning: 'VK_MAKE_API_VERSION' macro redefined [-Wmacro-redefined]
```

**Original Code**:
```cpp
#if defined(__ANDROID__) && !defined(VK_API_MAKE_VERSION)  // WRONG NAME
#define VK_MAKE_API_VERSION(variant, major, minor, patch) VK_MAKE_VERSION(major, minor, patch)
#endif
```

**Fixed Code**:
```cpp
#if defined(__ANDROID__) && !defined(VK_MAKE_API_VERSION)  // CORRECT NAME
#define VK_MAKE_API_VERSION(variant, major, minor, patch) VK_MAKE_VERSION(major, minor, patch)
#endif
```

**Explanation**:
- Guard was checking `VK_API_MAKE_VERSION` (doesn't exist)
- NDK r27 includes Vulkan 1.3 headers which define `VK_MAKE_API_VERSION`
- Correcting guard name prevents redefinition

## Code Structure

### Common Files (Shared with Chapter 1)

**Location**: `samples/openxr-tutorial/Common/` (symlinked from chapter2)

**Files**:
- `DebugOutput.h` - Android logging, stream redirection
- `GraphicsAPI.h` - Graphics API abstraction interface
- `GraphicsAPI_Vulkan.h/.cpp` - Vulkan implementation
- `OpenXRDebugUtils.h/.cpp` - Debug messenger utilities
- `HelperFunctions.h` - XrVersion utilities

**Note**: These files are **shared** between chapters. Changes affect all samples using them.

### Chapter 2 Specific Code

**File**: `main.cpp` (465 lines)

**New Classes/Methods**:
```cpp
class OpenXRTutorial {
    // Instance management
    void CreateInstance();
    void DestroyInstance();
    void GetInstanceProperties();

    // Debug utilities
    void CreateDebugMessenger();
    void DestroyDebugMessenger();

    // System identification
    void GetSystemID();

    // Session management
    void CreateSession();
    void DestroySession();

    // Event handling
    void PollEvents();

    // Main loop
    void Run();
};
```

**Event Handling** (lines 240-302):
```cpp
void PollEvents() {
    XrEventDataBuffer eventData{XR_TYPE_EVENT_DATA_BUFFER};
    while (xrPollEvent(m_xrInstance, &eventData) == XR_SUCCESS) {
        switch (eventData.type) {
            case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
                auto* stateChange = (XrEventDataSessionStateChanged*)&eventData;

                if (stateChange->state == XR_SESSION_STATE_READY) {
                    // Begin session
                    XrSessionBeginInfo beginInfo{XR_TYPE_SESSION_BEGIN_INFO};
                    beginInfo.primaryViewConfigurationType =
                        XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
                    xrBeginSession(m_session, &beginInfo);
                    m_sessionRunning = true;
                }

                if (stateChange->state == XR_SESSION_STATE_STOPPING) {
                    xrEndSession(m_session);
                    m_sessionRunning = false;
                }

                // Handle EXITING, LOSS_PENDING...
                break;
            }
            // Handle other events...
        }
    }
}
```

**No Explicit State Logging**: The original tutorial doesn't log state transitions. Session state changes happen automatically when `xrBeginSession()` is called.

## Testing Infrastructure

### test_run.bat

**Purpose**: One-command testing script

**Features**:
1. Device connectivity check
2. APK existence verification
3. Automated installation
4. App launch
5. Log collection after 5-second delay

**Usage**:
```bash
cd samples/openxr-tutorial/chapter2
.\test_run.bat
```

**Expected Output**:
```
========== Chapter 2 Application Logs ==========
openxr_tutorial: OpenXR Tutorial Chapter 2
openxr_tutorial: OpenXR Runtime: Oculus - 83.281.0
openxr_tutorial: OPENXR: Reference Space Change pending for Session: 0x3
```

### adb_cleanup.bat

**Purpose**: Clean uninstall

**Operations**:
1. Clear logcat buffer
2. Force-stop application
3. Uninstall package
4. Clear package data/cache

## Verification Checklist

- [x] APK builds without warnings
- [x] Native library `libopenxr_tutorial_ch2.so` created (1009448 bytes)
- [x] OpenXR loader `libopenxr_loader.so` included (1088400 bytes)
- [x] App launches on Quest 3 without crashes
- [x] OpenXR runtime identified: Oculus 83.281.0
- [x] Session created successfully (Session 0x3)
- [x] Event loop processing reference space changes
- [x] App displays black screen (expected - no rendering)
- [x] App exits cleanly when quit from Quest menu
- [x] No controller requirement dialog (focusaware flag working)

## Known Differences from Original Tutorial

### Intentional Changes

1. **Single Graphics API**: Vulkan only (no D3D/OpenGL variants)
   - Simplifies codebase for Quest-focused tutorial
   - Quest 3 performance is best with Vulkan

2. **Logging System**: Android-specific
   - Uses `__android_log_write` instead of platform-agnostic std::cout
   - Enables runtime debugging via adb logcat

3. **Build System**: Gradle + CMake
   - Original uses pure CMake
   - Android requires Gradle for APK packaging

4. **Dependency Management**: Maven + FetchContent
   - Original bundles OpenXR SDK
   - We fetch on-demand for easier updates

### Not Yet Implemented

1. **Multi-platform support**: Windows/Linux builds
   - Repository is Quest-focused
   - Can be added later if needed

2. **OpenGL ES variant**: Only Vulkan implemented
   - Quest 3 Vulkan performance superior
   - OpenGL ES can be added if needed for older devices

## Future Improvements

1. **Add explicit session state logging** (optional)
   ```cpp
   XR_TUT_LOG("Session state: " << stateChange->state);
   ```

2. **Performance profiling hooks**
   - Frame timing
   - CPU/GPU metrics

3. **Error recovery**
   - Handle instance loss gracefully
   - Session recreation on runtime restart

---

**Integration completed**: 2024-12-07
**Tested on**: Meta Quest 3 (Horizon OS v81)
**OpenXR Runtime**: Oculus 83.281.0
