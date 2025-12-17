# hello_xr Integration Notes

> Documentation of changes required to make OpenXR-SDK-Source hello_xr work standalone for Quest 3

**Date**: 2024-12-04
**Source**: [OpenXR-SDK-Source hello_xr](https://github.com/KhronosGroup/OpenXR-SDK-Source/tree/main/src/tests/hello_xr) release-1.1.54
**Target**: Meta Quest 3 (Horizon OS v81, Android API 34)

---

## Overview

The upstream `hello_xr` sample is designed to be built as part of the complete OpenXR-SDK-Source CMake project. This integration makes it buildable as a **standalone project** using CMake FetchContent.

## Critical Build Fixes

These changes were **required** to make the sample build and run:

### 1. Add Missing gfxwrapper Source File

**Problem**: Linker errors for undefined symbols `ksGpuWindow_Create` and `ksGpuWindow_Destroy`

**Files**:
- `common/gfxwrapper_opengl.c` (already present, not compiled)
- `common/gfxwrapper_opengl.h` (already present, not in headers list)

**Solution** - [CMakeLists.txt](CMakeLists.txt):
```cmake
set(LOCAL_HEADERS
    # ... existing headers ...
    common/gfxwrapper_opengl.h  # ADDED
)

set(LOCAL_SOURCE
    # ... existing sources ...
    common/gfxwrapper_opengl.c  # ADDED
)
```

**Upstream Reference**: In OpenXR-SDK-Source, these are compiled as part of the `openxr-gfxwrapper` library target (see `src/CMakeLists.txt:227`).

---

### 2. Update GLAD to GLAD2

**Problem**: Compile error - `gladLoaderLoadGLES2()` function not found

**Root Cause**: The tutorial initially had older GLAD files that didn't support OpenGL ES loaders.

**Solution**: Replace `glad/` directory contents with GLAD2 from upstream:
```bash
cp -r OpenXR-SDK-Source/src/external/glad2/include/* samples/hello_xr/glad/include/
cp -r OpenXR-SDK-Source/src/external/glad2/src/* samples/hello_xr/glad/src/
```

**Upstream Reference**: OpenXR-SDK-Source uses GLAD2 in `src/external/glad2/`

---

### 3. Enable OpenGL ES Support in GLAD

**Problem**: After GLAD2 update, `gladLoaderLoadGLES2()` still undefined

**Root Cause**: GLES2 loader functions are conditionally compiled based on `GLAD_GLES2` define.

**Solution** - [CMakeLists.txt:119](CMakeLists.txt#L119):
```cmake
target_compile_definitions(hello_xr PRIVATE
    XR_USE_PLATFORM_ANDROID
    XR_USE_GRAPHICS_API_OPENGL_ES
    XR_USE_GRAPHICS_API_VULKAN
    GLAD_GLES2  # ADDED - Enable OpenGL ES 2.0+ loader
)
```

**Upstream Reference**: See `src/external/glad2/CMakeLists.txt:33` - Android builds use `GLAD_GLES2`.

---

### 4. Add ANativeActivity Linker Flag

**Problem**: Runtime crash with `java.lang.UnsatisfiedLinkError: undefined symbol: ANativeActivity_onCreate`

**Root Cause**: Without the linker flag, the `ANativeActivity_onCreate` symbol is not exported from `libhello_xr.so`.

**Solution** - [CMakeLists.txt:166-168](CMakeLists.txt#L166-L168):
```cmake
# Set linker flag for native activity
set_target_properties(hello_xr PROPERTIES
    LINK_FLAGS "-u ANativeActivity_onCreate"
)
```

**Why Needed**: Forces the linker to retain the symbol even if it appears unused (it's called by Java NativeActivity framework via JNI).

**Upstream Reference**: The upstream uses a parent CMakeLists.txt that handles this automatically.

---

### 5. Add Quest 3 Metadata to AndroidManifest

**Problem**: Controllers and Meta button not working properly

**Root Cause**: Quest runtime needs explicit metadata to enable controller support and hand tracking.

**Solution** - [AndroidManifest.xml:50-55](AndroidManifest.xml#L50-L55):
```xml
<!-- Quest metadata: Enable hand tracking (optional, allows controllers OR hands) -->
<meta-data android:name="com.oculus.handtracking.frequency" android:value="HIGH" />
<meta-data android:name="com.oculus.handtracking.version" android:value="V2.0" />

<!-- Supported Quest devices -->
<meta-data android:name="com.oculus.supportedDevices" android:value="quest3|quest2|questpro" />
```

**Why Needed**:
- Without hand tracking metadata, Quest may not properly initialize controller input
- Without supported devices metadata, some Quest-specific features may not be available

**Reference**: See `hello_world` sample which has identical metadata and works correctly.

---

## Build System Changes

### CMake FetchContent for OpenXR SDK

**Upstream Approach**: Builds as part of parent OpenXR-SDK-Source CMake project

**Tutorial Approach**: Uses CMake FetchContent to download OpenXR SDK at configure time

**Implementation** - [CMakeLists.txt:12-22](CMakeLists.txt#L12-L22):
```cmake
include(FetchContent)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_CONFORMANCE_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_API_LAYERS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    OpenXR
    EXCLUDE_FROM_ALL
    URL https://github.com/KhronosGroup/OpenXR-SDK-Source/archive/refs/tags/release-1.1.54.tar.gz
    SOURCE_DIR openxr
)
FetchContent_MakeAvailable(OpenXR)
```

**Benefits**:
- Self-contained build (no parent project required)
- Version pinned to specific OpenXR release
- Easier for tutorial readers to understand

**Trade-offs**:
- Downloads ~10MB at first configure
- Builds `openxr_loader` from source (slower first build)

---

### Gradle Configuration

**Upstream**: Uses Gradle wrapper with version defined in parent project

**Tutorial**: Standalone Gradle project with pinned versions

**Key Settings** - [build.gradle](build.gradle):
```gradle
plugins {
    id 'com.android.application' version '8.13.0'
}

android {
    compileSdk 34
    ndkVersion "27.2.12479018"  // Updated from r23

    externalNativeBuild {
        cmake {
            path "CMakeLists.txt"  // Standalone CMake (not parent)
            version "3.22.1"       // Pinned CMake version
        }
    }
}
```

---

## Toolchain Updates

| Component | Upstream | Tutorial | Reason |
|-----------|----------|----------|--------|
| **NDK** | r23.2.8568313 | r27.2.12479018 | Quest 3 / Horizon OS v81 support |
| **Gradle** | 7.x | 8.13 | Android Studio compatibility |
| **AGP** | 7.x | 8.13.0 | Gradle 8 requirement |
| **CMake** | (parent) | 3.22.1 | NDK r27 bundled version |
| **API Level** | 24 | 24 (unchanged) | Vulkan minimum requirement |

---

## Development Utilities Added

### 1. Automated Testing Script

**File**: [test_run.bat](test_run.bat)

**Purpose**: One-command build → install → run → log workflow

**Usage**:
```bash
test_run.bat
```

**Workflow**:
1. Check ADB device connection
2. Build Vulkan debug APK
3. Install to Quest 3
4. Launch app
5. Stream logs in real-time
6. Cleanup on Ctrl+C

---

### 2. Device Cleanup Script

**File**: [adb_cleanup.bat](adb_cleanup.bat)

**Purpose**: Complete app removal including zombie processes and cached data

**Usage**:
```bash
adb_cleanup.bat
```

**Actions**:
1. Clear logcat buffer
2. Force-stop running app instances
3. Uninstall both Vulkan and OpenGL ES variants
4. Clear app data/cache

**Why Needed**: Quest sometimes leaves zombie icons after uninstall; this script ensures clean state.

---

## Documentation Added

### From Upstream (Copied As-Is)

1. **Application_Logic.md** (15KB)
   Complete walkthrough of hello_xr code flow and OpenXR session lifecycle

2. **Developers_Guidelines.md** (15KB)
   Development best practices for OpenXR applications

3. **Developers_Guidelines_Windows.md** (20KB)
   Windows-specific development environment setup

### Tutorial-Specific

4. **README.md** (Updated)
   Quest 3 build instructions, architecture overview, troubleshooting

5. **INTEGRATION_NOTES.md** (This file)
   Complete record of all changes from upstream

---

## Verification Checklist

After integrating hello_xr into a new project, verify:

- [x] **Build succeeds**: `gradlew assembleVulkanDebug` completes without errors
- [x] **APK installs**: `adb install -r build/outputs/apk/Vulkan/debug/hello_xr-Vulkan-debug.apk`
- [x] **App launches**: No crash, enters OpenXR session
- [x] **Controllers work**: Both left and right controllers tracked
- [x] **Meta button works**: Can quit app using Meta button
- [x] **Rendering works**: See two rotating cubes in stereo
- [x] **Logs clean**: No OpenXR errors in logcat

---

## Common Issues and Solutions

### Issue: `ksGpuWindow_Create` linker error

**Symptom**:
```
ld.lld: error: undefined symbol: ksGpuWindow_Create
>>> referenced by graphicsplugin_opengles.cpp:107
```

**Solution**: Add `common/gfxwrapper_opengl.c` to `LOCAL_SOURCE` in CMakeLists.txt

---

### Issue: `gladLoaderLoadGLES2` compile error

**Symptom**:
```
error: call to undeclared function 'gladLoaderLoadGLES2'
```

**Solution**:
1. Replace `glad/` with GLAD2 from upstream
2. Add `GLAD_GLES2` to compile definitions

---

### Issue: Runtime crash with `ANativeActivity_onCreate` undefined

**Symptom**:
```
java.lang.UnsatisfiedLinkError: undefined symbol: ANativeActivity_onCreate
```

**Solution**: Add linker flag:
```cmake
set_target_properties(hello_xr PROPERTIES
    LINK_FLAGS "-u ANativeActivity_onCreate"
)
```

---

### Issue: Meta button doesn't work

**Symptom**: App runs but Meta button doesn't trigger quit action

**Solution**: Add Quest metadata to AndroidManifest.xml:
```xml
<meta-data android:name="com.oculus.handtracking.frequency" android:value="HIGH" />
<meta-data android:name="com.oculus.handtracking.version" android:value="V2.0" />
<meta-data android:name="com.oculus.supportedDevices" android:value="quest3|quest2|questpro" />
```

---

## Gradle 9.0 Compatibility Fixes (2025-12-15)

These changes modernize the build configuration to eliminate deprecation warnings and prepare for Gradle 9.0.

### 1. Property Assignment Syntax

**Problem**: Groovy method-call syntax (e.g., `compileSdk 34`) is deprecated in AGP 8.x and will error in Gradle 9.0.

**Solution** - [build.gradle](build.gradle): Use assignment syntax (`=`) for all properties:

```gradle
// Before (deprecated)
compileSdk 34
ndkVersion "27.2.12479018"
abiFilters 'arm64-v8a'
debuggable true
flavorDimensions 'api'
dimension 'api'

// After (modern)
compileSdk = 34
ndkVersion = "27.2.12479018"
abiFilters = ['arm64-v8a']
debuggable = true
flavorDimensions = ['api']
dimension = 'api'
```

### 2. PackagingOptions Renamed

**Problem**: `packagingOptions` block is deprecated, replaced by `packaging`.

**Solution** - [build.gradle](build.gradle):
```gradle
// Before (deprecated)
packagingOptions {
    jniLibs {
        useLegacyPackaging = true
    }
}

// After (modern)
packaging {
    jniLibs {
        useLegacyPackaging = true
    }
}
```

### 3. Nested Resources Warning

**Problem**: Gradle detects nested resource directories and warns this will become an error in 9.0:
```
Nested resources detected.
+ android_resources
-- android_resources\vulkan
```

**Root Cause**: Main sourceset used `android_resources/` which contained flavor subdirectories `vulkan/` and `opengles/`.

**Solution**: Restructure resources to avoid nesting:

```
# Before (nested - triggers warning)
android_resources/
├── drawable/        ← main
├── values/          ← main
├── vulkan/          ← Vulkan flavor (NESTED!)
└── opengles/        ← OpenGLES flavor (NESTED!)

# After (flat - no warning)
android_resources/
├── common/          ← main sourceset
│   ├── drawable/
│   └── values/
├── vulkan/          ← Vulkan flavor
└── opengles/        ← OpenGLES flavor
```

**build.gradle change**:
```gradle
sourceSets {
    main {
        manifest.srcFile 'AndroidManifest.xml'
        res.srcDir 'android_resources/common'  // Changed from 'android_resources'
    }
    Vulkan {
        res.srcDir 'android_resources/vulkan'
    }
    OpenGLES {
        res.srcDir 'android_resources/opengles'
    }
}
```

---

## Future Maintenance

### Syncing with Upstream

To update hello_xr from a newer OpenXR-SDK-Source release:

1. **Copy source files** (these should be kept identical to upstream):
   - `main.cpp`
   - `openxr_program.cpp`
   - `openxr_program.h`
   - `graphicsplugin_*.cpp`
   - `platformplugin_*.cpp`
   - All header files

2. **DO NOT overwrite** (tutorial-specific modifications):
   - `CMakeLists.txt` (has FetchContent integration)
   - `AndroidManifest.xml` (has Quest metadata)
   - `build.gradle` (standalone configuration)

3. **Update GLAD if needed**:
   - Check if `src/external/glad2/` changed
   - Copy updated files to `glad/` directory

4. **Update documentation**:
   - Copy `Application_Logic.md`, `Developers_Guidelines*.md`
   - Update `README.md` with new upstream version number

5. **Verify build and runtime** using checklist above

---

## References

- **Upstream Source**: https://github.com/KhronosGroup/OpenXR-SDK-Source/tree/release-1.1.54/src/tests/hello_xr
- **OpenXR Spec 1.1**: https://registry.khronos.org/OpenXR/specs/1.1/html/xrspec.html
- **Quest Development**: https://developer.oculus.com/documentation/native/

---

**Maintained by**: OpenXR Quest Tutorial Project
**Last Updated**: 2025-12-15
