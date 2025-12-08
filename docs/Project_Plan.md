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
- [x] Implement and test Chapter 2 on Quest 3 (OpenXR instance, session, events) - **Complete 2024-12-07**
- [x] Implement and test Chapter 3 on Quest 3 (graphics rendering, swapchains, Vulkan) - **Complete 2024-12-07**
- [ ] Migrate and test Chapter 4 on Quest 3 (actions, interactions) - **In Progress** - [Detailed Plan](#chapter-4-actions--interactions-migration-plan)
- [ ] Implement and test Chapter 5 on Quest 3
- [ ] (Continue for remaining chapters)
- [ ] Identify duplications between samples
- [ ] Introduce common infrastructure if applicable

**Expected Output**: Progressive tutorial samples in `samples/openxr-tutorial/`

**Completed Chapters**:
- **Chapter 2**: See [samples/openxr-tutorial/chapter2/](../samples/openxr-tutorial/chapter2/) for OpenXR fundamentals implementation
- **Chapter 3**: See [samples/openxr-tutorial/chapter3/](../samples/openxr-tutorial/chapter3/) for Vulkan rendering and swapchain implementation

---

#### Chapter 4: Actions & Interactions (Migration Plan)

**Status**: 🔧 In Progress

**Source**: [OpenXR-Tutorials Chapter 4](https://github.com/KhronosGroup/OpenXR-Tutorials/releases/download/v1.0.13/Chapter4.zip)

**Additional Reference**: [openxr-tutorial.com - Chapter 4: Actions](https://openxr-tutorial.com/android/vulkan/4-actions.html)

**Goal**: Migrate OpenXR action system implementation for Quest 3 Touch Plus controller input, hand poses, and haptic feedback

**Expected Location**: `samples/openxr-tutorial/chapter4/`

##### Core Action System Features to Migrate

1. **Action Set Creation**
   - Create gameplay action set (pre-session initialization)
   - Define action set priority
   - Handle subaction paths for left/right hands

2. **Action Definitions**
   - Float input: Grip squeeze (analog 0.0-1.0)
   - Pose input: Hand/controller positions
   - Vibration output: Haptic feedback
   - Boolean input (if used): Button clicks

3. **Interaction Profile Bindings**
   - Suggest bindings for `/interaction_profiles/oculus/touch_controller`
   - Fallback bindings for `/interaction_profiles/khr/simple_controller`
   - Map semantic actions to physical inputs

4. **Action Space Management**
   - Create action spaces for pose actions (left/right hand)
   - Set identity `poseInActionSpace`
   - Query space locations in reference space

5. **Frame Loop Integration**
   - Sync actions with `xrSyncActions()` (after `xrWaitFrame()`)
   - Query action states (float, boolean, pose)
   - Locate action spaces for hand positions
   - Apply haptic feedback on events

6. **Controller Visualization**
   - Render hand markers (reuse Chapter 3 cube rendering)
   - Display controller positions in 3D space
   - Optional: Implement cube grabbing interaction

##### Implementation Steps

**Step 1: Source Download & Extraction**
- [x] Download Chapter 4 source: `https://github.com/KhronosGroup/OpenXR-Tutorials/releases/download/v1.0.13/Chapter4.zip`
- [x] Extract to temporary location
- [x] Review source structure:
  - `Chapter4/main.cpp` (main application with action system)
  - `Chapter4/GraphicsAPI_Vulkan.cpp` (existing from Chapter 3)
  - `Chapter4/Common/` (shared utilities)
  - `Chapter4/Android/` (Android build files if present)

**Step 2: Project Setup** (Similar to Chapter 3)
- [ ] Create `samples/openxr-tutorial/chapter4/` directory
- [ ] Copy Chapter 3's project structure as base:
  - `app/build.gradle`
  - `settings.gradle`
  - `gradle.properties`
  - `gradlew`, `gradlew.bat`
  - `gradle/wrapper/`
- [ ] Update `app/build.gradle`:
  - Change `applicationId` to `com.example.openxr_tutorial_ch4`
  - Update `project.name` references
- [ ] Update `settings.gradle`:
  - `rootProject.name = 'OpenXRTutorialChapter4'`
- [ ] Create symlink to `Common/` directory:
  - From `samples/openxr-tutorial/Common/` (relative path)
  - Or copy Common files if symlink issues on Windows
- [ ] Copy `AndroidManifest.xml` from Chapter 3, update package name

**Step 3: Source Code Migration**

Main application code:
- [ ] Copy `main.cpp` from Chapter 4 source to project root
- [ ] Review differences from Chapter 3's `main.cpp`:
  - Action set creation code (pre-session)
  - Action definitions (grab, pose, haptic)
  - Interaction profile bindings
  - Action space creation (session phase)
  - Session action sets attachment
  - Frame loop sync and query logic
- [ ] Adapt Android-specific code:
  - JNI integration (if needed)
  - Asset loading (if controller models used)
  - Logging (`__android_log_print` vs `std::cout`)

GraphicsAPI files:
- [ ] Copy `GraphicsAPI_Vulkan.cpp` from Chapter 4 (likely same as Chapter 3)
- [ ] Copy `GraphicsAPI.h` header
- [ ] Verify no new graphics features (Chapter 4 focuses on input, not rendering)

Common files:
- [ ] Verify `Common/` symlink/copy includes:
  - `HelperFunctions.h` (XrPath utilities, matrix math)
  - `GraphicsAPI.h` interface
  - `DebugOutput.h` (logging)
  - `OpenXRDebugUtils.h` (XR_EXT_debug_utils)

Shader files:
- [ ] Copy `VertexShader.glsl` from Chapter 4 (likely unchanged from Chapter 3)
- [ ] Copy `PixelShader.glsl` (check for per-view tinting)
- [ ] Verify no shader changes needed for Chapter 4

**Step 4: CMakeLists.txt Configuration**
- [ ] Copy base CMakeLists.txt from Chapter 3
- [ ] Update project name:
  ```cmake
  project(openxr_tutorial_ch4 VERSION 1.0.0 LANGUAGES CXX)
  ```
- [ ] Verify source files list includes Chapter 4 code:
  - `main.cpp`
  - `GraphicsAPI_Vulkan.cpp`
- [ ] Keep shader compilation from Chapter 3:
  - `glslc` compilation for `.glsl` → `.spv`
  - Output to `.cxx/Debug/*/assets/shaders/`
- [ ] Verify OpenXR headers fetched (unchanged from Chapter 3)
- [ ] Link Vulkan loader from Maven (unchanged)

**Step 5: Gradle Build Configuration**
- [ ] Update `app/build.gradle`:
  - Verify `applicationId` is `com.example.openxr_tutorial_ch4`
  - Update CMake `arguments` if needed (unlikely)
  - Keep shader asset packaging from Chapter 3:
    ```gradle
    sourceSets {
        main {
            assets {
                srcDirs += [
                    "$projectDir/.cxx/Debug/.../arm64-v8a",
                    "$projectDir/.cxx/Release/.../arm64-v8a"
                ]
            }
        }
    }
    ```
  - Verify native library name matches CMakeLists.txt

**Step 6: AndroidManifest.xml Adaptation**
- [ ] Copy from Chapter 3, update package to `com.example.openxr_tutorial_ch4`
- [ ] Update native library name:
  ```xml
  <meta-data android:name="android.app.lib_name" android:value="openxr_tutorial_ch4" />
  ```
- [ ] Verify Quest metadata (based on Chapter 3 lessons):
  - **Keep minimal config** to avoid controller popup:
    - `com.oculus.supportedDevices` metadata
    - Remove `com.oculus.vr.focusaware` if present
    - Remove `com.oculus.handtracking.enabled` if present
    - Remove `com.oculus.permission.HAND_TRACKING` permission
  - Controllers work by default (no extra permissions needed)
- [ ] Set appropriate `android:label`:
  ```xml
  android:label="OpenXR Ch4 Actions"
  ```

**Step 7: Action System Code Review & Quest Adaptations**

Review and adapt action creation code:
- [ ] **Action Set Creation**:
  - Verify action set created before session
  - Log action set creation success
  - Handle errors with `XR_FAILED(result)`

- [ ] **Action Definitions**:
  - Float input for grip squeeze:
    ```cpp
    XrActionCreateInfo grabInfo{XR_TYPE_ACTION_CREATE_INFO};
    grabInfo.actionType = XR_ACTION_TYPE_FLOAT_INPUT;
    strcpy(grabInfo.actionName, "grab_cube");
    strcpy(grabInfo.localizedActionName, "Grab Cube");
    grabInfo.countSubactionPaths = handPaths.size();
    grabInfo.subactionPaths = handPaths.data();
    xrCreateAction(actionSet, &grabInfo, &grabAction);
    ```
  - Pose input for hand tracking
  - Vibration output for haptics

- [ ] **Quest Touch Controller Bindings**:
  - Grip squeeze: `/user/hand/left/input/squeeze/value`
  - Grip pose: `/user/hand/left/input/grip/pose`
  - Haptic: `/user/hand/left/output/haptic`
  - Log active interaction profile:
    ```cpp
    xrGetCurrentInteractionProfile(session, leftHandPath, &profileState);
    // Should be: /interaction_profiles/oculus/touch_controller
    ```

- [ ] **Fallback KHR Simple Controller**:
  - Select input: `/user/hand/left/input/select/click`
  - Grip pose: `/user/hand/left/input/grip/pose`
  - Haptic: `/user/hand/left/output/haptic`

- [ ] **Action Space Creation**:
  - Create after session, before action set attachment
  - Use identity `poseInActionSpace`
  - Store left/right hand spaces for frame loop queries

- [ ] **Attach Action Sets to Session**:
  - Call once after session creation
  - Cannot be called multiple times (OpenXR limitation)
  - Verify success before entering frame loop

**Step 8: Frame Loop Integration**

Sync and query actions:
- [ ] **xrSyncActions()** after `xrWaitFrame()`:
  ```cpp
  XrActiveActionSet activeActionSet{};
  activeActionSet.actionSet = gameplayActionSet;
  activeActionSet.subactionPath = XR_NULL_PATH;
  XrActionsSyncInfo syncInfo{XR_TYPE_ACTIONS_SYNC_INFO};
  syncInfo.countActiveActionSets = 1;
  syncInfo.activeActionSets = &activeActionSet;
  xrSyncActions(session, &syncInfo);
  ```

- [ ] **Query Float Action** (grip squeeze):
  ```cpp
  XrActionStateGetInfo getInfo{XR_TYPE_ACTION_STATE_GET_INFO};
  getInfo.action = grabCubeAction;
  getInfo.subactionPath = leftHandPath;
  XrActionStateFloat grabState{XR_TYPE_ACTION_STATE_FLOAT};
  xrGetActionStateFloat(session, &getInfo, &grabState);
  if (grabState.isActive && grabState.currentState > 0.5f) {
      // Grip squeezed
  }
  ```

- [ ] **Query Pose Action** (hand position):
  ```cpp
  XrSpaceLocation handLocation{XR_TYPE_SPACE_LOCATION};
  xrLocateSpace(leftHandSpace, localSpace, predictedDisplayTime, &handLocation);
  if (handLocation.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) {
      // Use handLocation.pose for rendering
  }
  ```

- [ ] **Apply Haptic Feedback**:
  ```cpp
  XrHapticActionInfo hapticInfo{XR_TYPE_HAPTIC_ACTION_INFO};
  hapticInfo.action = buzzAction;
  hapticInfo.subactionPath = leftHandPath;
  XrHapticVibration vibration{XR_TYPE_HAPTIC_VIBRATION};
  vibration.amplitude = 0.8f;
  vibration.duration = XrDuration(50 * 1000000); // 50ms
  vibration.frequency = XR_FREQUENCY_UNSPECIFIED;
  xrApplyHapticFeedback(session, &hapticInfo, (XrHapticBaseHeader*)&vibration);
  ```

**Step 9: Controller Visualization**

Render hand markers:
- [ ] Reuse Chapter 3's `RenderCube()` function
- [ ] Convert `XrPosef` to transformation matrix:
  ```cpp
  glm::mat4 XrPoseToMat4(const XrPosef& pose) {
      glm::quat rotation(pose.orientation.w, pose.orientation.x,
                         pose.orientation.y, pose.orientation.z);
      glm::vec3 translation(pose.position.x, pose.position.y, pose.position.z);
      return glm::translate(glm::mat4(1.0f), translation) * glm::mat4_cast(rotation);
  }
  ```
- [ ] Render small colored cubes at hand positions:
  - Left hand: Green marker
  - Right hand: Red marker
  - Scale: ~0.05m (5cm cube)
- [ ] Render in frame loop after scene cubes

**Step 10: Testing Infrastructure**
- [ ] Copy `test_run.bat` from Chapter 3
- [ ] Update package name to `com.example.openxr_tutorial_ch4`
- [ ] Update log tag to `openxr_tutorial_ch4`
- [ ] Add action-specific log filters:
  ```batch
  adb logcat -d | findstr /I "action grip squeeze haptic"
  ```
- [ ] Copy `adb_cleanup.bat` from Chapter 3
- [ ] Update package name in cleanup script

**Step 11: Build & Deploy Testing**
- [ ] Run `gradlew assembleVulkanDebug` from chapter4 directory
- [ ] Verify APK created: `app/build/outputs/apk/vulkan/debug/app-vulkan-debug.apk`
- [ ] Check APK contents for shaders:
  ```batch
  7z l app-vulkan-debug.apk | findstr .spv
  ```
- [ ] Install on Quest 3 via `test_run.bat`
- [ ] Verify no controller popup (app launches directly)
- [ ] Check logcat for action system initialization:
  - Action set created
  - Interaction profile logged
  - Action spaces created
  - Action sets attached

**Step 12: Functional Verification on Quest 3**

Input testing:
- [ ] **Grip Squeeze Detection**:
  - Put on headset, squeeze grip on each controller
  - Verify logs show squeeze values (0.0-1.0)
  - Check left/right hand distinction

- [ ] **Hand Pose Tracking**:
  - Move controllers around
  - Verify green/red markers track hands
  - Check markers appear at correct 3D positions
  - Verify both eyes see markers (stereo rendering)

- [ ] **Haptic Feedback**:
  - Squeeze grip past threshold (>0.5)
  - Feel controller vibration
  - Verify only active controller vibrates
  - Check amplitude feels appropriate

- [ ] **Interaction Profile**:
  - Check logs for active profile
  - Should be `/interaction_profiles/oculus/touch_controller`
  - Verify no fallback to simple controller

- [ ] **Frame Rate Stability**:
  - Verify 72Hz maintained (no drops)
  - Check no jitter in controller tracking
  - Ensure haptics don't cause frame drops

**Step 13: Quest 3 Specific Adaptations Documentation**
- [ ] Create `INTEGRATION_NOTES.md`:
  - Action system setup differences from original
  - Touch controller binding paths used
  - Haptic parameter tuning (amplitude, duration)
  - Interaction profile selection logic
  - Asset loading (if any models added)
  - Known differences from desktop tutorial

- [ ] Document manifest adaptations:
  - Why minimal metadata used
  - Controller permission handling
  - Differences from Chapter 3

- [ ] Note coordinate system consistency:
  - Hand poses use same reference space as Chapter 3 cubes
  - Right-handed Y-up coordinate system
  - Grip pose vs aim pose distinction

**Step 14: Documentation**

Create comprehensive documentation:
- [ ] **README.md**:
  - Overview of action system features
  - Build instructions (`gradlew assembleVulkanDebug`)
  - Deployment instructions (`test_run.bat`)
  - Expected behavior:
    - Chapter 3 cubes still render
    - Green/red markers track controllers
    - Grip squeeze triggers haptic feedback
  - Troubleshooting:
    - Action `isActive` always false
    - Haptics not working
    - Controller markers at wrong positions
    - Controller popup appears
  - Quest Touch controller input reference

- [ ] **Code Comments**:
  - Document action set creation flow
  - Explain interaction profile binding strategy
  - Clarify frame loop action sync timing
  - Note haptic feedback parameters

- [ ] **Update Project_Plan.md**:
  - Mark Chapter 4 steps as complete
  - Update completion date
  - Add reference to Chapter 4 documentation

**Step 15: Cleanup and Resource Management**
- [ ] Verify action cleanup in destructor:
  ```cpp
  // Destroy in reverse order of creation
  for (auto& space : handSpaces) {
      if (space != XR_NULL_HANDLE) xrDestroySpace(space);
  }
  if (grabCubeAction != XR_NULL_HANDLE) xrDestroyAction(grabCubeAction);
  if (palmPoseAction != XR_NULL_HANDLE) xrDestroyAction(palmPoseAction);
  if (buzzAction != XR_NULL_HANDLE) xrDestroyAction(buzzAction);
  if (gameplayActionSet != XR_NULL_HANDLE) xrDestroyActionSet(gameplayActionSet);
  ```
- [ ] Test app exit (back button from Quest menu)
- [ ] Verify no OpenXR handle leaks in logs
- [ ] Test repeated launches (ensure clean state)

##### Platform-Specific Considerations

**AndroidManifest.xml**:
- Use minimal metadata (learned from Chapter 3)
- Controllers available by default (no extra permissions)
- Update package name to `com.example.openxr_tutorial_ch4`
- Update library name to `openxr_tutorial_ch4`

**Quest Touch Plus Controller Inputs**:
- Analog grip: `/input/squeeze/value` (0.0-1.0)
- Digital grip: `/input/squeeze/click` (threshold ~0.8)
- Grip pose: `/input/grip/pose` (handle center)
- Aim pose: `/input/aim/pose` (forward ray)
- Haptic: `/output/haptic` (single motor per controller)
- Buttons: A, B, X, Y, menu, thumbstick (not used in basic tutorial)

**Logging**:
- Use `__android_log_print` for all logging
- Log action set creation, binding suggestions, profile selection
- Log grip squeeze values (throttle to avoid spam)
- Log hand positions for debugging (once per second max)

**Build System**:
- Reuse Chapter 3's shader compilation (no changes)
- Verify Gradle asset packaging still works
- CMake FetchContent for OpenXR headers (unchanged)
- Maven dependency for Vulkan loader (unchanged)

**Known Differences from Original Tutorial**:
- Tutorial may assume desktop (Windows/Linux)
- Quest version uses Android asset loading
- Logging uses Android APIs
- Interaction profile specific to Touch controllers
- Haptic parameters tuned for Quest hardware

##### Success Criteria

Chapter 4 is complete when:
- [ ] App builds without errors
- [ ] App installs and launches on Quest 3 (no controller popup)
- [ ] Chapter 3 cubes still render correctly
- [ ] Controller markers (green/red) visible and tracking hands
- [ ] Grip squeeze detected and logged (analog 0.0-1.0)
- [ ] Haptic feedback triggers on squeeze (felt in controller)
- [ ] Active interaction profile is `oculus/touch_controller`
- [ ] Frame rate stable at 72Hz (no performance regression)
- [ ] App exits cleanly via Quest menu
- [ ] Documentation complete (README.md, INTEGRATION_NOTES.md)
- [ ] Test scripts updated and working (`test_run.bat`, `adb_cleanup.bat`)

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
