# Chapter 1 Integration Notes

> Detailed adaptation notes for Quest 3 implementation

## Overview

This document details the changes made to adapt the original OpenXR Tutorial Chapter 1 for Meta Quest 3 with our repository's build system.

**Original Source**: [openxr-tutorial.com - Chapter 1: Introduction](https://openxr-tutorial.com/android/vulkan/1-introduction.html)

**Reference Implementation**: Based on OpenXR-Tutorials repository structure (similar to Chapter 2)

## Build System Configuration

### 1. OpenXR SDK Integration

**Approach**: Hybrid Maven + CMake FetchContent

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

**Rationale**:
- Headers from FetchContent (build-time dependency)
- Loader from Maven (runtime library via Prefab)
- No manual SDK installation required

### 2. Native Library Configuration

**File**: `app/build.gradle`

```gradle
android {
    namespace 'com.example.openxr_tutorial_ch1'

    defaultConfig {
        applicationId "com.example.openxr_tutorial_ch1"

        externalNativeBuild {
            cmake {
                arguments "-DANDROID_STL=c++_static",
                          "-DANDROID_ARM_NEON=TRUE",
                          "-DXR_USE_PLATFORM_ANDROID=ON",
                          "-DXR_USE_GRAPHICS_API_VULKAN=ON"
            }
        }

        ndk {
            abiFilters 'arm64-v8a'
        }
    }

    packagingOptions {
        jniLibs.useLegacyPackaging = true
    }
}
```

**Key Points**:
- Unique namespace/applicationId for isolation
- CMake defines platform/API macros at build time
- arm64-v8a only (Quest 3 requirement)
- Legacy packaging for NativeActivity

### 3. CMake Build Configuration

**File**: `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.22.1)
project(openxr_tutorial_ch1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch OpenXR headers
include(FetchContent)
FetchContent_Declare(
    openxr
    GIT_REPOSITORY https://github.com/KhronosGroup/OpenXR-SDK-Source.git
    GIT_TAG release-1.1.54
)
FetchContent_MakeAvailable(openxr)

# Main application library
add_library(openxr_tutorial_ch1 SHARED
    ../main.cpp
)

target_include_directories(openxr_tutorial_ch1 PRIVATE
    ${openxr_SOURCE_DIR}/include
    ${openxr_SOURCE_DIR}/src/common
    ../Common
)

target_link_libraries(openxr_tutorial_ch1
    openxr::openxr_loader
    android
    log
    vulkan
)

target_compile_definitions(openxr_tutorial_ch1 PRIVATE
    XR_USE_PLATFORM_ANDROID
    XR_USE_GRAPHICS_API_VULKAN
)
```

**Note**: Chapter 1 is minimal - no OpenXR API calls yet, just project structure.

## Platform Configuration

### AndroidManifest.xml

**File**: `app/src/main/AndroidManifest.xml`

```xml
<manifest xmlns:android="http://schemas.android.com/apk/res/android">
    <!-- Vulkan requirement -->
    <uses-feature
        android:name="android.hardware.vulkan.version"
        android:version="0x00400003"
        android:required="true" />

    <!-- 6DOF tracking -->
    <uses-feature
        android:name="android.hardware.vr.headtracking"
        android:version="1"
        android:required="true" />

    <application android:hasCode="false">
        <!-- Hand tracking -->
        <meta-data android:name="com.oculus.handtracking.frequency" android:value="HIGH" />
        <meta-data android:name="com.oculus.handtracking.version" android:value="V2.0" />

        <!-- Supported devices -->
        <meta-data android:name="com.oculus.supportedDevices"
                   android:value="quest3|quest2|questpro" />

        <activity android:name="android.app.NativeActivity">
            <meta-data android:name="android.app.lib_name"
                       android:value="openxr_tutorial_ch1" />

            <!-- OpenXR intents -->
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
                <category android:name="org.khronos.openxr.intent.category.IMMERSIVE_HMD" />
            </intent-filter>

            <!-- Quest-specific -->
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
                <category android:name="com.oculus.intent.category.VR" />
            </intent-filter>
        </activity>
    </application>
</manifest>
```

## Code Structure

### Main Application

**File**: `main.cpp` (183 lines)

**Purpose**: Minimal OpenXR tutorial entry point - establishes Android/OpenXR integration without actual OpenXR API usage.

**Key Components**:

1. **OpenXRTutorial Class** (empty placeholder):
```cpp
class OpenXRTutorial {
public:
    OpenXRTutorial(GraphicsAPI_Type apiType) {}
    ~OpenXRTutorial() = default;
    void Run() {}  // Empty - Chapter 1 has no OpenXR calls
};
```

2. **Android Platform Integration**:
```cpp
static android_app *androidApp = nullptr;
struct AndroidAppState {
    ANativeWindow *nativeWindow = nullptr;
    bool resumed = false;
};

static void AndroidAppHandleCmd(android_app *app, int32_t cmd) {
    // Handle APP_CMD_INIT_WINDOW, APP_CMD_TERM_WINDOW, etc.
}

void PollSystemEvents() {
    // Process Android app events
}
```

3. **OpenXR Loader Initialization** (android_main):
```cpp
void android_main(struct android_app *app) {
    // Attach JNI thread
    JNIEnv *env;
    app->activity->vm->AttachCurrentThread(&env, nullptr);

    // Initialize OpenXR loader (required for Android)
    XrInstance m_xrInstance = XR_NULL_HANDLE;
    PFN_xrInitializeLoaderKHR xrInitializeLoaderKHR = nullptr;

    xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrInitializeLoaderKHR",
                          (PFN_xrVoidFunction *)&xrInitializeLoaderKHR);

    XrLoaderInitInfoAndroidKHR loaderInitInfo{XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR};
    loaderInitInfo.applicationVM = app->activity->vm;
    loaderInitInfo.applicationContext = app->activity->clazz;
    xrInitializeLoaderKHR((XrLoaderInitInfoBaseHeaderKHR *)&loaderInitInfo);

    // Set callbacks
    app->userData = &OpenXRTutorial::androidAppState;
    app->onAppCmd = OpenXRTutorial::AndroidAppHandleCmd;

    OpenXRTutorial::androidApp = app;
    OpenXRTutorial_Main(XR_TUTORIAL_GRAPHICS_API);
}
```

### Common Files

**Location**: `samples/openxr-tutorial/Common/` (symlinked)

**Files Used by Chapter 1**:
- `DebugOutput.h` - Android logging macros (XR_TUT_LOG, XR_TUT_LOG_ERROR)
- `GraphicsAPI.h` - Graphics API type definitions
- `HelperFunctions.h` - XrVersion utilities

**Files NOT Used** (but present for future chapters):
- `GraphicsAPI_Vulkan.h/.cpp`
- `OpenXRDebugUtils.h/.cpp`

## Android Logging

**Implementation**: `Common/DebugOutput.h` (lines 98-165)

```cpp
#ifdef __ANDROID__
class AndroidStreambuf : public std::streambuf {
    android_LogPriority logPriority;
    // ... buffer management ...

    int sync() {
        // Write to Android log when newline encountered
        __android_log_write(logPriority, "openxr_tutorial", str.c_str());
    }
};

class DebugOutput {
    AndroidStreambuf androidCout;
    AndroidStreambuf androidCerr;

    DebugOutput() {
        std::cout.rdbuf(&androidCout);
        std::cerr.rdbuf(&androidCerr);

        __android_log_write(ANDROID_LOG_DEBUG, "openxr_tutorial",
                           "redirected cout");
        std::cout << "Testing cout redirect." << std::endl;
    }
};
#endif
```

**Log Tag**: `"openxr_tutorial"` (same for all chapters)

## Testing Infrastructure

### test_run.bat

**Purpose**: Automated build-deploy-test script

**Operations**:
1. Check Quest connectivity
2. Verify APK build
3. Install via ADB
4. Launch application
5. Collect logs after 3-second delay

**Expected Logs**:
```
openxr_tutorial: redirected cout
openxr_tutorial: Testing cout redirect.
openxr_tutorial: OpenXR Tutorial Chapter 1
```

**Note**: Chapter 1 doesn't create OpenXR instance - just validates loader initialization.

### adb_cleanup.bat

**Purpose**: Clean device state

**Operations**:
1. Clear logcat
2. Force-stop app
3. Uninstall package
4. Clear app data

## Differences from Original Tutorial

### Intentional Changes

1. **Loader Initialization**: Added explicit `xrInitializeLoaderKHR` for Android
   - **Why**: Required for Android OpenXR applications
   - **Where**: android_main() function

2. **Log Redirection**: Android-specific std::cout/cerr handling
   - **Why**: Make debug output visible in logcat
   - **Where**: DebugOutput.h

3. **Build System**: Gradle + CMake instead of pure CMake
   - **Why**: Android APK packaging requires Gradle
   - **Where**: build.gradle + CMakeLists.txt

4. **Dependency Management**: Maven + FetchContent hybrid
   - **Why**: Leverage Android ecosystem standards
   - **Where**: build.gradle (loader), CMakeLists.txt (headers)

### Maintained from Original

- Empty OpenXRTutorial class (placeholder for future chapters)
- Android event handling structure
- Graphics API type system (even though unused in Chapter 1)

## Build Verification

### Expected Outputs

**Build Success**:
```
BUILD SUCCESSFUL in Xs
```

**APK Contents**:
```bash
$ unzip -l app/build/outputs/apk/vulkan/debug/app-vulkan-debug.apk | grep lib
  <size>  lib/arm64-v8a/libopenxr_loader.so
  <size>  lib/arm64-v8a/libopenxr_tutorial_ch1.so
```

**Native Library**:
- `libopenxr_tutorial_ch1.so` (minimal size, ~50KB)
- `libopenxr_loader.so` (from Maven, ~1MB)

### Runtime Verification

**Expected Behavior**:
1. App launches on Quest 3
2. Black screen displayed (no rendering)
3. Logs show loader initialization
4. App stays running until manually quit

**Logcat Output**:
```
openxr_tutorial: redirected cout
openxr_tutorial: Testing cout redirect.
openxr_tutorial: OpenXR Tutorial Chapter 1
```

**No OpenXR Runtime Logs** (instance not created yet)

## Future Integration

Chapter 1 establishes:
-  Android platform integration
-  OpenXR loader initialization
-  Build system configuration
-  Logging infrastructure

Chapter 2 adds:
- OpenXR instance creation
- Debug messenger
- System/session management
- Event polling

## Known Limitations

1. **No Visual Output**: Chapter 1 is structure-only
2. **No OpenXR Validation**: Loader initializes but instance never created
3. **Empty Run Loop**: Placeholder for future chapters

These are intentional - Chapter 1 focuses on project setup.

---

**Integration completed**: 2024-12-06
**Tested on**: Meta Quest 3 (Horizon OS v81)
**Build verified**: APK builds and deploys successfully
