# Meta OpenXR SDK - Integration Notes

> **Build fixes, hand tracking guidelines, and microgestures usage**
>
> Last updated: 2025-12-19

---

## Table of Contents

1. [Build Fixes](#build-fixes)
   - [NDK 29 Compatibility Fixes](#ndk-29-compatibility-fixes)
   - [XrHandTrackingWideMotionMode Internal API Fix](#xrhandtrackingwidemotionmode-internal-api-fix)
2. [Hand Tracking Guidelines](#hand-tracking-guidelines)
   - [Controller vs Hand Tracking Switching](#controller-vs-hand-tracking-switching)
   - [Enabling Hand Tracking](#enabling-hand-tracking)
   - [UI Interaction with Hands](#ui-interaction-with-hands)
3. [Microgestures Guidelines](#microgestures-guidelines)
   - [Hand Position](#hand-position)
   - [Swipe Gestures](#swipe-gestures)
   - [Thumb Tap Gesture](#thumb-tap-gesture)
   - [Tips for Reliable Detection](#tips-for-reliable-detection)
4. [XrInput Sample](#xrinput-sample)
   - [Overview](#overview)
   - [Picking Up the Tool](#picking-up-the-tool)
   - [Using the Tool](#using-the-tool-while-holding)
   - [Action Sets Architecture](#action-sets-architecture)
   - [Known Code Issues](#known-code-issues)
5. [XrSpaceWarp Sample](#xrspacewarp-sample)
   - [What is Application SpaceWarp](#what-is-application-spacewarp)
   - [What You See](#what-you-see)
   - [Controls](#controls-1)
   - [How to Observe the Effect](#how-to-observe-the-effect)
6. [XrSceneSharing Sample](#xrscenesharing-sample)
   - [Prerequisites](#prerequisites)
   - [Controls](#controls-2)
   - [Workflow](#workflow)

---

## Build Fixes

### NDK 29 Compatibility Fixes

The Meta samples have been upgraded to use **NDK 29.0.14206865** with **C++ 20**. This required several source code and build configuration fixes for compatibility with the newer toolchain.

#### 0. CMake C++ Standard Upgrade

**Change:** Updated `Samples/CMakeLists.txt` line 22 from C++17 to C++20:

```cmake
# Before
set(CMAKE_CXX_STANDARD 17)

# After
set(CMAKE_CXX_STANDARD 20)
```

This enables C++20 features like `std::to_array` which the SDK source code uses.

#### 1. ALooper_pollAll Deprecation

**Problem:** `ALooper_pollAll` was deprecated in NDK 29 and marked as unavailable. The function may ignore wakes and has been replaced with `ALooper_pollOnce`.

```
error: 'ALooper_pollAll' is unavailable: obsoleted in Android 1 - ALooper_pollAll may ignore wakes. Use ALooper_pollOnce instead.
```

**Fix:** Replace `ALooper_pollAll` with `ALooper_pollOnce` in all affected files:

**Affected Files (8 total):**
- `Samples/SampleXrFramework/Src/XrApp.cpp:1465`
- `Samples/XrSamples/XrPassthrough/Src/XrPassthrough.cpp:1157`
- `Samples/XrSamples/XrPassthroughOcclusion/Src/XrPassthroughOcclusion.cpp:1187`
- `Samples/XrSamples/XrSpatialAnchor/Src/SpatialAnchorXr.cpp:1971`
- `Samples/XrSamples/XrSceneModel/Src/SceneModelXr.cpp:2203`
- `Samples/XrSamples/XrSceneSharing/Src/SceneSharingXr.cpp:2055`
- `Samples/XrSamples/XrCompositor_NativeActivity/Src/XrCompositor_NativeActivity.c:3485`
- `Samples/XrSamples/XrSpaceWarp/Src/XrSpaceWarp.c:3224`

```cpp
// Before
if (ALooper_pollAll(timeoutMilliseconds, NULL, &events, (void**)&source) < 0) {

// After
if (ALooper_pollOnce(timeoutMilliseconds, NULL, &events, (void**)&source) < 0) {
```

#### 2. ARM32-Only Compiler Flags

**Problem:** The 3rdParty/CMakeLists.txt included `-marm` and `-mfpu=neon` flags which are only valid for 32-bit ARM (armeabi-v7a), not 64-bit ARM (arm64-v8a/aarch64).

```
clang: error: unsupported option '-mno-thumb' for target 'aarch64-none-linux-android29'
clang: error: unsupported option '-mfpu=' for target 'aarch64-none-linux-android29'
```

**Fix:** Make the 32-bit ARM flags conditional on ABI in `Samples/3rdParty/CMakeLists.txt`:

```cmake
# Before
if(ANDROID)
    target_compile_options(minizip PRIVATE
        $<$<COMPILE_LANGUAGE:C>:-Wno-unused-command-line-argument;-marm;-mfpu=neon>
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-unused-command-line-argument;-marm>
    )

# After
if(ANDROID)
    target_compile_options(minizip PRIVATE
        $<$<COMPILE_LANGUAGE:C>:-Wno-unused-command-line-argument>
        $<$<COMPILE_LANGUAGE:CXX>:-Wno-unused-command-line-argument>
    )
    # 32-bit ARM specific flags (not valid for aarch64)
    if(ANDROID_ABI STREQUAL "armeabi-v7a")
        target_compile_options(minizip PRIVATE
            $<$<COMPILE_LANGUAGE:C>:-marm;-mfpu=neon>
            $<$<COMPILE_LANGUAGE:CXX>:-marm>
        )
    endif()
```

#### 3. Literal Operator Whitespace Deprecation

**Problem:** C++23 deprecates whitespace between `""` and the suffix in user-defined literal operators. With `-Werror`, this becomes an error in clang 18+ (NDK 29).

```
error: identifier '_cm' preceded by whitespace in a literal operator declaration is deprecated [-Werror,-Wdeprecated-literal-operator]
   55 | constexpr float operator"" _cm(long double centimeters) {
      |                 ~~~~~~~~~~~^~~
```

**Fix:** Remove whitespace between `""` and the suffix in all literal operator declarations:

**Affected Files (3 total):**
- `Samples/XrSamples/XrInput/Src/main.cpp` (lines 55, 58, 63, 66)
- `Samples/XrSamples/XrHandsAndControllers/Src/main.cpp` (lines 52, 55, 60, 63)
- `Samples/XrSamples/XrMicrogestures/Src/main.cpp` (lines 31, 34)

```cpp
// Before
constexpr float operator"" _cm(long double centimeters) {
constexpr float operator"" _m(long double meters) {

// After
constexpr float operator""_cm(long double centimeters) {
constexpr float operator""_m(long double meters) {
```

---

### XrHandTrackingWideMotionMode Internal API Fix

**Problem:** The `XrHandTrackingWideMotionMode` sample contains internal/unreleased code that was supposed to be stripped from the public SDK (marked with `/// BEGIN_SDK_REMOVE` / `/// END_SDK_REMOVE` comments) but wasn't. This code references undefined APIs:

- `wideMotionModeSource_` member variable (not declared)
- `WideMotionModeSource()` method (not defined)
- `XR_HAND_TRACKING_WIDE_MOTION_MODE_SOURCE_INFERENCE_METAX1` constant (not in public headers)

```
error: use of undeclared identifier 'wideMotionModeSource_'
error: no member named 'WideMotionModeSource' in 'XrHandHelper'
error: use of undeclared identifier 'XR_HAND_TRACKING_WIDE_MOTION_MODE_SOURCE_INFERENCE_METAX1'
```

**Affected Files:**
- `Samples/XrSamples/XrHandTrackingWideMotionMode/Src/xr_hand_helper.h`
- `Samples/XrSamples/XrHandTrackingWideMotionMode/Src/main.cpp`

**Fix for xr_hand_helper.h (lines 133-138):**
```cpp
// Before
virtual bool Update(XrSpace currentSpace, XrTime predictedDisplayTime) override {
    if (xrLocateHandJointsEXT_) {
        /// aim
        aimState_.next = nullptr;
        /// BEGIN_SDK_REMOVE
        wideMotionModeSource_.next = nullptr;
        aimState_.next = &wideMotionModeSource_;
        /// END_SDK_REMOVE
        /// scale
        scale_.next = &aimState_;

// After (remove the SDK_REMOVE block)
virtual bool Update(XrSpace currentSpace, XrTime predictedDisplayTime) override {
    if (xrLocateHandJointsEXT_) {
        /// aim
        aimState_.next = nullptr;
        /// scale
        scale_.next = &aimState_;
```

**Fix for main.cpp:** Remove all 4 blocks between `/// BEGIN_SDK_REMOVE` and `/// END_SDK_REMOVE` (around lines 319-358):

```cpp
// Before
if (hand_Base_L_->AreLocationsActive() && hand_Base_L_->IsPositionValid()) {
    /// Render solid Hands
    handRenderer_Base_L_.Solidity = 1.0f;
    /// BEGIN_SDK_REMOVE
    if(hand_Base_L_->WideMotionModeSource().wideMotionSource == XR_HAND_TRACKING_WIDE_MOTION_MODE_SOURCE_INFERENCE_METAX1){
        handRenderer_Base_L_.Solidity = 0.25f;
    }
    /// END_SDK_REMOVE
    handRenderer_Base_L_.Render(out.Surfaces);
}

// After
if (hand_Base_L_->AreLocationsActive() && hand_Base_L_->IsPositionValid()) {
    /// Render solid Hands
    handRenderer_Base_L_.Solidity = 1.0f;
    handRenderer_Base_L_.Render(out.Surfaces);
}
```

Repeat for all 4 occurrences (Base_L, Base_R, WMM_L, WMM_R).

---

## Hand Tracking Guidelines

### Controller vs Hand Tracking Switching

The hand tracking samples (e.g., `XrHandsFB`) implement **automatic switching** between controllers and hands:

```cpp
// Controllers render ONLY when hands are NOT tracked
if (in.LeftRemoteTracked && !handTrackedL_) {
    controllerRenderL_.Update(in.LeftRemotePose);
}

// Hands render ONLY when actively tracked
if (handTrackedL_) {
    handRendererL_.Render(out.Surfaces);
}
```

**Key behavior:**
- When you **hold controllers**, the system switches to controller tracking mode
- `locationsL.isActive` / `locationsR.isActive` become `false` when controllers are active
- Hands appear briefly when you put down controllers, then disappear if you pick them up again

### Enabling Hand Tracking

1. **Quest Settings:** Go to **Settings > Movement Tracking > Hand and Body Tracking**
2. Ensure **Hand Tracking** is **ON**
3. **Put down both controllers** - set them on a table or somewhere not touching your hands
4. **Wait 1-2 seconds** for Quest to switch from controller to hand tracking mode

### UI Interaction with Hands

Hand tracking samples use **pinch gestures** for UI interaction, not physical buttons:

```cpp
const bool didPinch = (aimState.status & XR_HAND_TRACKING_AIM_INDEX_PINCHING_BIT_FB) != 0;
ui_.AddHitTestRay(FromXrPosef(aimState.aimPose), didPinch && !lastFrameClicked);
```

**To interact with UI:**
1. Point at a button with your hand
2. **Pinch** your index finger and thumb together to "click"
3. Release to complete the interaction

---

## Microgestures Guidelines

Microgestures are "low-calorie" thumb motions performed on the **side of the index finger**. They provide D-pad-like directional commands.

### Hand Position

**Critical for reliable detection:**
- Hold your hand **sideways** (like you're about to shake hands)
- Fingers should be **slightly curled** - not extended flat, not in a fist
- Think of a relaxed, natural grip position
- Other fingers should be relaxed, not tense

### Swipe Gestures

Your **thumb slides along the SIDE of your index finger**:

```
                    FORWARD (away from palm)
                         ^
                         |
        LEFT <------ [INDEX FINGER] ------> RIGHT
        (toward                    (toward
         fingertip)                 thumb base)
                         |
                         v
                    BACKWARD (toward palm)
```

| Gesture | Motion |
|---------|--------|
| **Swipe Left** | Thumb slides toward index fingertip (along the side) |
| **Swipe Right** | Thumb slides toward thumb base (along the side) |
| **Swipe Forward** | Thumb slides away from palm (toward back of hand) |
| **Swipe Backward** | Thumb slides toward palm |

### Thumb Tap Gesture

1. Touch the **middle segment** of your index finger with your thumb
2. **Lift** the thumb away
3. The gesture triggers on the **release**, not the touch

### Tips for Reliable Detection

1. **Small motions:** Swipes are only **2-3 cm** - don't exaggerate
2. **Relaxed hand:** Keep fingers slightly curled, not tense
3. **Consistent position:** Maintain the "handshake" orientation
4. **Self-haptic feedback:** You should feel your thumb touching your index finger
5. **Practice:** The gestures are subtle and require some learning
6. **Similar to smartphone:** Think of it like scrolling on a phone screen

### Supported Devices

- Quest 2
- Quest 3
- Quest Pro

### Required SDK Version

Meta XR Core SDK Package **v74 or higher**

---

## XrInput Sample

### Overview

This sample demonstrates the **OpenXR Action System** - not general pointing/grabbing mechanics. It teaches how Actions, ActionSets, and Spaces work together. The sample includes a cube-building tool that showcases action set priorities and context switching.

**Key design point:** There are no persistent pointer beams. Beams only appear when pointing at UI panels.

### Picking Up the Tool

The black cube-shaped **tool** spawns at `{0m, -0.3m, -0.5m}` (30cm below eye level, 50cm in front of you).

**To grab it:**
1. Move your controller **close to the tool** (within 17cm hitbox)
2. Press the **grip/squeeze button** (side button), NOT the trigger
3. The controller model disappears and is replaced by the tool

### Using the Tool (While Holding)

**Touch Controller:**

| Input | Action |
|-------|--------|
| Trigger (tool hand) | Place a cube at template position |
| Left thumbstick | Rotate the template cube |
| Right thumbstick up/down | Move template cube closer/farther |
| Right thumbstick left/right | Scale the template cube |
| A button | Change cube color (Red/Green/Blue) |
| Grip button | Drop the tool |

**Hand Tracking Controls:**

| Input | Action |
|-------|--------|
| Pinch (tool hand) | Place cube |
| Distance between hands (off-hand pinched) | Scale template cube |
| Menu button | Drop tool |

### UI Interaction

Beams appear only when pointing at UI panels. Use them to click buttons like "Clear placed cubes". The beams do NOT appear while holding the tool (due to action set priority).

### Action Sets Architecture

The sample uses three action sets with different priorities:

| Action Set | Priority | Purpose |
|------------|----------|---------|
| Menu | 0 | UI selection and clicking |
| World | 0 | Base actions (grab/drop tool, grip pose) |
| Tool | 9 (higher) | Cube spawning controls (only active while holding tool) |

The higher priority of the Tool action set means it overrides Menu actions while the tool is held - this is why you can't click UI buttons while holding the tool.

### Known Code Issues

**Copy-paste bug at `Src/main.cpp:849`:**
```cpp
bool menuBeamActiveLeft = ActionPoseIsActive(actionMenuBeamPose_, rightHandPath_);  // Should be leftHandPath_
```

Both lines 849 and 858 check `rightHandPath_`, so the left hand UI beam may not function correctly. The right hand beam works fine.

---

## XrSpaceWarp Sample

### What is Application SpaceWarp

**Application SpaceWarp (ASW)** is a frame extrapolation technology - NOT a visual space-warping effect. The name can be misleading.

**How it works:**
- Application renders at **36 FPS** (half the normal rate)
- Application provides **motion vectors** and **depth buffer** to the runtime
- Runtime extrapolates/reprojects frames to **72 FPS**
- Result: Smooth experience while reducing GPU load by ~50%

This is useful for GPU-intensive applications that struggle to maintain full framerate.

### What You See

| Element | Purpose |
|---------|---------|
| **Jumping cubes** | Fast-moving objects to demonstrate motion vector quality |
| **Ground plane** | Static reference surface |
| **Controller visualization** | Shows tracked controller positions |

The "jumping boxes" are intentional - they move quickly to stress-test the motion vector system.

### Controls

| Input | Action |
|-------|--------|
| **Trigger (HOLD)** | **Disable** SpaceWarp → Normal 72 FPS rendering |
| **Trigger (RELEASE)** | **Enable** SpaceWarp → 36 FPS with extrapolation |
| **Left Thumbstick L/R** | Rotate camera around Y axis |
| **Grip/Squeeze** | Move scene on X axis |
| **Trigger Value** | Move scene on Y axis |
| **Thumbstick Click** | Additional toggle |

### How to Observe the Effect

1. **Look at the jumping cubes while moving your head**
2. **Hold the trigger** (SpaceWarp OFF) - observe rendering at 72 FPS
3. **Release the trigger** (SpaceWarp ON) - now rendering at 36 FPS but extrapolated

**What to look for:**
- The difference is subtle when motion vectors are working correctly
- Look for artifacts at object edges during fast head movement
- If you see no difference, that means SpaceWarp is working well!

**Debug options** (via adb):
```bash
adb shell setprop debug.oculus.enableSpaceWarp 0  # Force disable
adb shell setprop debug.oculus.enableSpaceWarp 1  # Force enable
```

---

## XrSceneSharing Sample

This sample demonstrates **colocated multi-user experiences** using shared spatial anchors. It requires pre-existing room scan data.

### Prerequisites

1. **Developer Mode enabled** on Quest
2. **Space Setup completed** (room scan exists)
3. **Permissions granted**: Spatial data, scene access, anchor API
4. For sharing features: Multiple Quest devices on same network

### Controls

| Button | Action | Role |
|--------|--------|------|
| **A** | Query local scene (rooms with room layout) | Host |
| **B** | Share queried scene to group UUID file | Host |
| **X** | Load shared scene from group UUID file | Guest |
| **Y** | Query room layout anchors only (walls/floor/ceiling) | Both |
| **Left Trigger** | Toggle plane visualization mode | Both |
| **Right Trigger** | **Request Scene Capture (opens Space Setup)** | Both |

### Workflow

**If you only see grid lines and axes:**
1. You likely don't have a room scan yet
2. Press **Right Trigger** to launch Space Setup
3. Complete the room scan
4. Press **A** to query your scene

**For multi-user sharing:**
1. **Host**: Press A → query scene → Press B → share to file
2. **Guest**: Press X → load from shared group UUID file

### Troubleshooting

- **Buttons don't work**: No room scan exists - use Right Trigger first
- **B does nothing**: Query must complete first (press A and wait)
- **X fails**: No group UUID file exists (Host must share first)
- **No scene data**: Check Settings > Apps > SceneSharing > Permissions

---

## References

- [Meta Developer Docs - Hand Tracking Microgestures](https://developers.meta.com/horizon/documentation/unity/unity-microgestures/)
- [Meta Developer Docs - Microgestures Locomotion](https://developers.meta.com/horizon/documentation/unity/unity-isdk-locomotion-microgestures/)
- [OpenXR Hand Tracking Extension](https://registry.khronos.org/OpenXR/specs/1.0/html/xrspec.html#XR_EXT_hand_tracking)

---

## Samples Status

| Sample | Status | Notes |
|--------|--------|-------|
| XrPassthrough | Works | - |
| XrPassthroughOcclusion | Works | - |
| XrHandDataSource | Works | After std::to_array fix |
| XrHandsFB | Works | Put down controllers to see hands |
| XrHandTrackingWideMotionMode | Works | After internal API removal fix |
| XrMicrogestures | Works | Requires correct hand position for swipes |
| XrInput | Works | Grab tool with grip button, not trigger; beams only on UI |
| XrSpaceWarp | Works | Hold trigger to disable; shows frame extrapolation tech |
| XrSceneSharing | Works | Requires room scan first; Right Trigger for Space Setup |

---

*Document created from integration session on 2025-12-17*
