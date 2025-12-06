# OpenXR Quest Tutorial - Implementation Plan

> **Repository**: `openxr-quest-tutorial`
> **Target**: Meta Quest 3 (Horizon OS v81, Android API 34)

## Current Status

**Completed**: Phases 0, 1, 2, 3
**In Progress**: [Phase 4: OpenXR Tutorial Migration](#phase-4-openxr-tutorial-migration)
**Next**: [Phase 5: Meta SDK Samples](#phase-5-meta-sdk-samples)

## Implementation Phases

### Phase 0: Prerequisites

**Status**: ✅ Completed

- Install development tools (Android Studio, NDK r27, Java 17, Python 3.14)
- Configure environment (see [Environment_Setup.md](Environment_Setup.md))
- OpenXR SDK managed via Maven + CMake FetchContent

### Phase 1: Repository Foundation

**Status**: ✅ Completed

- Repository structure established
- Documentation framework created
- ADR infrastructure added ([docs/adrs/](adrs/))
- Build configuration established

### Phase 2: hello_world Sample

**Status**: ✅ Completed

**Documentation**: [samples/hello_world/](../samples/hello_world/)

### Phase 3: hello_xr Sample

**Status**: ✅ Completed (2024-12-04)

**Source**: [OpenXR-SDK-Source hello_xr](https://github.com/KhronosGroup/OpenXR-SDK-Source/tree/main/src/tests/hello_xr)

**Steps**:
1. Copy source files from OpenXR-SDK-Source release-1.1.54
2. Update CMakeLists.txt (CMake FetchContent for headers)
3. Update build.gradle (Maven dependency for loader)
4. Fix missing gfxwrapper files
5. Update GLAD to GLAD2 with OpenGL ES support
6. Add Quest metadata to AndroidManifest.xml
7. Create automated test scripts (test_run.bat, adb_cleanup.bat)
8. Document integration changes (INTEGRATION_NOTES.md)

**Result**: Standalone hello_xr sample building and running on Quest 3

**Documentation**: [samples/hello_xr/](../samples/hello_xr/)

### Phase 4: OpenXR Tutorial Migration

**Status**: 🔧 In Progress

**Source**: [openxr-tutorial.com Android/Vulkan Tutorial](https://openxr-tutorial.com/android/vulkan/1-introduction.html)

**Goal**: Adapt the progressive tutorial series for Quest 3 with our build system

**Steps**:
- [x] Implement and test Chapter 1 on Quest 3 (setup - coordinate with hello_world/hello_xr patterns)
- [ ] Implement and test Chapter 2 on Quest 3 - [Detailed Plan](#chapter-2-detailed-action-plan)
- [ ] Implement and test Chapter 3 on Quest 3
- [ ] Implement and test Chapter 4 on Quest 3
- [ ] Implement and test Chapter 5 on Quest 3
- [ ] (Continue for remaining chapters)
- [ ] Identify duplications between samples
- [ ] Introduce common infrastructure if applicable

**Expected Output**: Progressive tutorial samples in `samples/openxr-tutorial/`

#### Chapter 2: Detailed Action Plan

**Reference**: [openxr-tutorial.com - Chapter 2: Setup](https://openxr-tutorial.com/android/vulkan/2-setup.html#openxr-setup)

**Tutorial Source Files**:
- Downloaded from [GitHub Release v1.0.13](https://github.com/KhronosGroup/OpenXR-Tutorials/releases/download/v1.0.13/Chapter2.zip)
- Location: `C:\Users\PRO\Projects\openxr-quest-tutorial\temp_chapter2\Chapter2\`

**Objective**: Implement core OpenXR infrastructure including instance creation, system identification, session management, and event polling.

##### Implementation Tasks

**1. Project Structure Setup** ✅
- [x] Create `samples/openxr-tutorial/chapter2/` directory structure
- [x] Copy and adapt Android project structure from chapter1
- [x] Update `settings.gradle` in root to include chapter2
- [x] Create chapter2-specific `README.md`

**2. Main Application Code (`main.cpp`)**

Adapt from reference file `temp_chapter2/Chapter2/main.cpp` (465 lines)

**2.1. Update OpenXRTutorial Class Constructor**
- [ ] Add `m_apiType` member variable initialization
- [ ] Add graphics API validation call: `CheckGraphicsAPI_TypeIsValidForPlatform()`
- [ ] Update constructor to handle API type parameter

**2.2. Implement Instance Management**
- [ ] Add `CreateInstance()` method with:
  - `XrApplicationInfo` structure population
  - API layer enumeration via `xrEnumerateApiLayerProperties()`
  - Instance extension enumeration via `xrEnumerateInstanceExtensionProperties()`
  - Extension validation (required: `XR_EXT_DEBUG_UTILS_EXTENSION_NAME`, graphics API extension)
  - `xrCreateInstance()` call with validated layers/extensions
- [ ] Update `DestroyInstance()` method
- [ ] Add `GetInstanceProperties()` method to log runtime information

**2.3. Implement Debug Utilities**
- [ ] Add `CreateDebugMessenger()` method
  - Check for `XR_EXT_debug_utils` in active extensions
  - Call `CreateOpenXRDebugUtilsMessenger()` from OpenXRDebugUtils.h
- [ ] Add `DestroyDebugMessenger()` method
  - Call `DestroyOpenXRDebugUtilsMessenger()` from OpenXRDebugUtils.h

**2.4. Implement System Identification**
- [ ] Add `GetSystemID()` method with:
  - `XrSystemGetInfo` structure (form factor: `XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY`)
  - `xrGetSystem()` call to obtain `XrSystemId`
  - `xrGetSystemProperties()` call to retrieve hardware capabilities

**2.5. Implement Session Management**
- [ ] Add `CreateSession()` method with:
  - Graphics API instantiation (Vulkan for Android/Quest)
  - `std::unique_ptr<GraphicsAPI_Vulkan>` creation
  - `XrSessionCreateInfo` structure with graphics binding
  - `xrCreateSession()` call
- [ ] Add `DestroySession()` method with `xrDestroySession()` call

**2.6. Implement Event Polling**
- [ ] Add `PollEvents()` method with event loop handling:
  - `XR_TYPE_EVENT_DATA_EVENTS_LOST` - log lost event count
  - `XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING` - trigger shutdown
  - `XR_TYPE_EVENT_DATA_INTERACTION_PROFILE_CHANGED` - log profile changes
  - `XR_TYPE_EVENT_DATA_REFERENCE_SPACE_CHANGE_PENDING` - log space changes
  - `XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED` - **critical state machine**:
    - `XR_SESSION_STATE_READY` → call `xrBeginSession()`, set `m_sessionRunning = true`
    - `XR_SESSION_STATE_STOPPING` → call `xrEndSession()`, set `m_sessionRunning = false`
    - `XR_SESSION_STATE_EXITING` → shutdown application
    - `XR_SESSION_STATE_LOSS_PENDING` → shutdown application

**2.7. Update Main Application Loop**
- [ ] Update `Run()` method sequence:
  ```cpp
  CreateInstance();
  CreateDebugMessenger();
  GetInstanceProperties();
  GetSystemID();
  CreateSession();

  while (m_applicationRunning) {
      PollSystemEvents();
      PollEvents();
      if (m_sessionRunning) {
          // Placeholder for future frame rendering
      }
  }

  DestroySession();
  DestroyDebugMessenger();
  DestroyInstance();
  ```

**2.8. Add Required Member Variables**
- [ ] Add to OpenXRTutorial class private section:
  ```cpp
  // Instance & Extensions
  XrInstance m_xrInstance = XR_NULL_HANDLE;
  std::vector<const char*> m_activeAPILayers = {};
  std::vector<const char*> m_activeInstanceExtensions = {};
  std::vector<std::string> m_apiLayers = {};
  std::vector<std::string> m_instanceExtensions = {};

  // Debug
  XrDebugUtilsMessengerEXT m_debugUtilsMessenger = XR_NULL_HANDLE;

  // System
  XrFormFactor m_formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
  XrSystemId m_systemID = {};
  XrSystemProperties m_systemProperties = {XR_TYPE_SYSTEM_PROPERTIES};

  // Graphics API
  GraphicsAPI_Type m_apiType = UNKNOWN;
  std::unique_ptr<GraphicsAPI> m_graphicsAPI = nullptr;

  // Session
  XrSession m_session = {};
  XrSessionState m_sessionState = XR_SESSION_STATE_UNKNOWN;
  bool m_applicationRunning = true;
  bool m_sessionRunning = false;
  ```

**2.9. Android Platform Integration**
- [ ] Keep existing Android-specific code from chapter1:
  - `static android_app *androidApp`
  - `struct AndroidAppState`
  - `static void AndroidAppHandleCmd()`
  - `void PollSystemEvents()` (Android-specific implementation)
  - `void android_main()` with loader initialization

**3. Common Files Updates**

Check if chapter1 Common files need updates by comparing with `temp_chapter2/Common/`:

- [ ] Compare `GraphicsAPI.h` - verify `CheckGraphicsAPI_TypeIsValidForPlatform()` exists
- [ ] Compare `GraphicsAPI_Vulkan.h` - verify `GetGraphicsBinding()` method
- [ ] Compare `GraphicsAPI_Vulkan.cpp` - verify constructor accepts `(XrInstance, XrSystemId)`
- [ ] Compare `OpenXRDebugUtils.h` - verify messenger create/destroy functions
- [ ] Update Common files if discrepancies found

**4. Build Configuration**

**4.1. CMakeLists.txt**
- [ ] Copy from chapter1 and update:
  - Project name: `openxr_tutorial_ch2`
  - Library name: `openxr_tutorial_ch2`
  - Verify OpenXR SDK FetchContent (release-1.1.54)
  - Verify Vulkan library linkage
  - Ensure `XR_USE_PLATFORM_ANDROID` and `XR_USE_GRAPHICS_API_VULKAN` are defined

**4.2. app/build.gradle**
- [ ] Copy from chapter1 and update:
  - `namespace 'com.example.openxr_tutorial_ch2'`
  - `applicationId "com.example.openxr_tutorial_ch2"`
  - Verify Maven dependency: `org.khronos.openxr:openxr_loader_for_android:1.1.54`
  - Verify Prefab support enabled
  - Verify `useLegacyPackaging = true` for NativeActivity
  - Maintain Vulkan/OpenGL ES flavor dimensions

**4.3. AndroidManifest.xml**
- [ ] Copy from chapter1 and update:
  - Update `android:label` to "OpenXR Tutorial Ch2"
  - Verify Quest-specific metadata (hand tracking, passthrough permissions)
  - Ensure NativeActivity configuration

**5. Testing & Validation**

**5.1. Build Process**
- [ ] Build Vulkan variant: `gradlew :samples:openxr-tutorial:chapter2:assembleVulkanDebug`
- [ ] Verify no compilation errors
- [ ] Check native library output: `libopenxr_tutorial_ch2.so`

**5.2. Deployment & Runtime Testing**
- [ ] Install APK on Quest 3 via ADB
- [ ] Launch application and capture logcat output
- [ ] Verify expected log messages:
  - OpenXR instance creation success
  - Runtime name/version (should show "Oculus" or Meta runtime)
  - System properties (Quest 3 hardware info)
  - Session creation success
  - Event loop operation
  - Session state transitions: IDLE → READY → SYNCHRONIZED → VISIBLE → FOCUSED

**5.3. Expected Behavior**
- Application should:
  - Launch without crashing
  - Create OpenXR instance and session successfully
  - Enter the main event loop
  - Respond to session state changes
  - Display black screen (no rendering yet - that's Chapter 3)
  - Exit cleanly when user quits via Quest menu

**5.4. Create Test Scripts**
- [ ] Create `test_run.bat` (following chapter1 pattern)
- [ ] Create `adb_cleanup.bat` for uninstall/log cleanup

**6. Documentation**

- [ ] Create `samples/openxr-tutorial/chapter2/README.md` with:
  - Overview of Chapter 2 concepts
  - Build & deploy instructions
  - Key differences from original tutorial
  - Link to reference tutorial
  - Prerequisites
  - Next chapter reference

- [ ] Create `samples/openxr-tutorial/chapter2/INTEGRATION_NOTES.md` documenting:
  - Changes from original Chapter2.zip reference code
  - Quest 3 specific adaptations
  - Maven/Prefab integration details
  - Common file modifications (if any)

##### Success Criteria

- ✅ Application builds without errors for Vulkan variant
- ✅ Application deploys to Quest 3 successfully
- ✅ OpenXR instance created and debug messenger active
- ✅ System ID and properties retrieved successfully
- ✅ OpenXR session created with Vulkan graphics binding
- ✅ Event polling loop operational
- ✅ Session state machine transitions correctly
- ✅ Application responds to session lifecycle events
- ✅ Clean shutdown on exit
- ✅ Logcat shows expected OpenXR runtime messages
- ✅ No memory leaks or crashes during lifecycle

##### Reference Files Location

**Temporary reference files** (for implementation):
- `C:\Users\PRO\Projects\openxr-quest-tutorial\temp_chapter2\Chapter2\main.cpp` (primary reference)
- `C:\Users\PRO\Projects\openxr-quest-tutorial\temp_chapter2\Common\*.{h,cpp}` (utility comparisons)

**Tutorial documentation**:
- Web: https://openxr-tutorial.com/android/vulkan/2-setup.html
- Original source: https://github.com/KhronosGroup/OpenXR-Tutorials/releases/tag/v1.0.13

---

### Phase 5: Meta SDK Samples

**Status**: ⏳ Planned

**Source**: OVR OpenXR Mobile SDK 81.0 samples

**Goal**: Import Meta-specific OpenXR extension samples

**Key Samples** (selection TBD):
- XrPassthrough
- XrHandsFB
- XrControllers
- XrSpatialAnchor
- (Additional samples as needed)

**Steps**:
1. Copy SampleXrFramework to `samples/meta/_framework/`
2. Select and copy relevant Meta samples
3. Update CMakeLists.txt for each sample
4. Update build.gradle configurations
5. Add to root settings.gradle
6. Document Meta extensions used

**Expected Output**: Meta extension samples in `samples/meta/`

### Phase 6: Automation & Documentation

**Status**: ⏳ Planned

**Goal**: Python automation scripts and comprehensive documentation

**Steps**:
1. Create `scripts/build.py` (context-aware building)
2. Create `scripts/deploy.py` (context-aware deployment)
3. Create `scripts/setup_env.py` (environment verification)
4. Complete documentation in `docs/`
5. Add sample-specific analysis documents

**Expected Output**: Automated workflows and complete documentation
