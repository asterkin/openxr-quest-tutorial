# Developer Guidelines for OpenXR hello_xr on Meta Quest 3

> **For native Windows PC VR development, see**: [Developers_Guidelines_Windows.md](Developers_Guidelines_Windows.md)

---

## Prerequisites

- **Android Studio** (optional, for debugging only)
- **Gradle 8.13** (included via gradlew.bat)
- **Android Gradle Plugin 8.13.0** (configured in build.gradle)
- **ADB** (Android Debug Bridge)
- **Meta Quest Developer Hub (MQDH) 6.1.1+**
- **Meta Quest 3** with Developer Mode enabled

---

## Build Configuration

### Important Version Fix

The original sample used AGP 8.13.1 which doesn't exist. Ensure `build.gradle` uses:

```gradle
plugins {
    id 'com.android.application' version '8.13.0'
}
```

### Product Flavors

Two graphics API variants:
- **OpenGLES** - Broader compatibility
- **Vulkan** - Recommended for Quest 3 (better performance)

---

## Recommended Gradle Commands

### Build Tasks

```cmd
:: List all available tasks
gradlew.bat tasks
gradlew.bat tasks --all

:: Build only (no install)
gradlew.bat assembleVulkanDebug
gradlew.bat assembleVulkanRelease

:: Build and install to connected Quest
gradlew.bat installVulkanDebug
gradlew.bat installVulkanRelease

:: Uninstall
gradlew.bat uninstallVulkanDebug
gradlew.bat uninstallAll

:: Clean build artifacts
gradlew.bat clean

:: Full rebuild and install
gradlew.bat clean assembleVulkanDebug installVulkanDebug
```

### Output Locations

- Debug APK: `build/outputs/apk/Vulkan/debug/hello_xr-Vulkan-debug.apk`
- Release APK: `build/outputs/apk/Vulkan/release/hello_xr-Vulkan-release.apk`

---

## ADB Commands

### Device Management

```cmd
:: List connected devices
adb devices
adb devices -l

:: Device info
adb shell getprop ro.product.model
adb shell getprop ro.build.version.release

:: Reboot device
adb reboot
```

### App Installation

```cmd
:: Install APK
adb install build\outputs\apk\Vulkan\debug\hello_xr-Vulkan-debug.apk

:: Reinstall (faster for updates, keeps data)
adb install -r build\outputs\apk\Vulkan\debug\hello_xr-Vulkan-debug.apk

:: Force install (ignores version conflicts)
adb install -r -d build\outputs\apk\Vulkan\debug\hello_xr-Vulkan-debug.apk
```

### App Lifecycle

```cmd
:: Check if app is installed
adb shell pm list packages | findstr hello_xr
adb shell pm list packages | findstr khronos

:: Launch app (Method 1 - NativeActivity)
adb shell am start -n org.khronos.openxr.hello_xr.vulkan/android.app.NativeActivity

:: Launch app (Method 2 - Monkey, simpler)
adb shell monkey -p org.khronos.openxr.hello_xr.vulkan 1

:: Launch app (Method 3 - Main intent)
adb shell am start -a android.intent.action.MAIN -c android.intent.category.LAUNCHER org.khronos.openxr.hello_xr.vulkan

:: Stop app
adb shell am force-stop org.khronos.openxr.hello_xr.vulkan

:: Uninstall
adb uninstall org.khronos.openxr.hello_xr.vulkan

:: Uninstall but keep app data
adb uninstall -k org.khronos.openxr.hello_xr.vulkan
```

### Logging

```cmd
:: View all logs
adb logcat

:: Clear logs and monitor
adb logcat -c && adb logcat

:: Filter by app
adb logcat | findstr "hello_xr"
adb logcat | findstr "OpenXR"

:: Filter by severity
adb logcat *:E *:W
adb logcat *:E

:: Save logs to file
adb logcat > hello_xr_logs.txt

:: Monitor crashes
adb logcat | findstr "FATAL"
adb logcat | findstr "AndroidRuntime"

:: OpenXR runtime logs
adb logcat -s "OVR"

:: Vulkan validation layers
adb logcat -s "VALIDATION"
```

### Wireless ADB (Optional)

```cmd
:: 1. Connect via USB first
adb tcpip 5555

:: 2. Get Quest IP address (in Quest: Settings → Wi-Fi → Network Details)
:: 3. Connect wirelessly (replace with your Quest's IP)
adb connect 192.168.1.XXX:5555

:: 4. Verify connection
adb devices

:: 5. Disconnect USB cable (wireless connection active)

:: To disconnect wireless
adb disconnect
```

---

## Meta Quest Developer Hub (MQDH)

### Key Features for Development

#### Device Actions Panel

1. **Cast Device**
   - Click "Start casting" for live screen mirroring
   - Shows VR view on PC in real-time
   - **Tip**: Disable "Proximity Sensor" to cast without wearing headset

2. **Record Video**
   - Capture VR session for review/documentation
   - Saved to MQDH's recording directory

3. **Screenshot**
   - One-time capture of current VR view
   - Requires headset to be worn (unless proximity sensor disabled)

4. **GPU Profiling Service**
   - Toggle "On" to enable performance overlay in VR
   - Shows FPS, frame time, GPU/CPU metrics in real-time
   - Essential for performance optimization

5. **OVR Metrics Tool**
   - Click "Install" to add advanced performance HUD
   - Displays FPS, latency, thermal data, CPU/GPU usage
   - Overlaid in VR for real-time monitoring

6. **VrRuntime Debugging Tool**
   - Configure OpenXR runtime debugging options
   - Useful for troubleshooting runtime issues

#### Apps Tab

- View all installed apps (including sideloaded)
- **Launch App** dropdown - Start VR apps directly from MQDH
- **Upload Build** - Install APK from file picker
- **Uninstall** - Remove apps via context menu (⋮)
- Shows package name, version, install date

#### Device Logs Tab

- Real-time logcat viewer with filtering
- Equivalent to `adb logcat` but with GUI
- Can filter by severity, package, or custom tags

### MQDH vs CLI

**Use MQDH for:**
- Live screen casting (essential for VR development)
- Performance profiling (GPU metrics, OVR metrics)
- Quick app launching
- Visual log viewing with filters

**Use CLI for:**
- Automated build/deploy scripts
- CI/CD pipelines
- Quick iteration (Gradle + ADB one-liners)
- Advanced log filtering with grep/findstr

---

## Recommended Development Workflow

### Standard Iteration Cycle

```cmd
:: Option 1: Gradle (builds + installs)
gradlew.bat installVulkanDebug

:: Option 2: Separate build and install
gradlew.bat assembleVulkanDebug
adb install -r build\outputs\apk\Vulkan\debug\hello_xr-Vulkan-debug.apk

:: Launch via ADB
adb shell monkey -p org.khronos.openxr.hello_xr.vulkan 1

:: Monitor logs
adb logcat -c && adb logcat | findstr "hello_xr\|OpenXR\|ERROR"
```

### One-Liner Deploy + Launch + Logs

```cmd
gradlew.bat installVulkanDebug && adb shell monkey -p org.khronos.openxr.hello_xr.vulkan 1 && timeout /t 2 && adb logcat -c && adb logcat | findstr "hello_xr"
```

### With MQDH Screen Casting

1. Start MQDH casting (Device Actions → Cast device → Start casting)
2. Run build/install/launch command
3. View VR output on PC monitor
4. Monitor logs in MQDH or terminal

---

## Quest UI Behavior for Sideloaded Apps

### App Icon Visibility

**Expected behavior:**
- Pure OpenXR apps (like hello_xr) use `NativeActivity` and may not appear in standard app launcher
- App is installed and functional even without visible icon
- Icon appears in **Unknown Sources** section after **first launch**
- Icon may persist as "ghost" after uninstall (Quest OS caching bug)

### Ghost Icon Fix (Development Only)

If uninstalled app icons persist:

```cmd
adb shell pm clear com.oculus.vrshell
adb shell pm clear com.oculus.systemux
adb reboot
```

**Note**: This clears Quest's launcher cache. Ghost icons are a sideloading quirk and **do not affect store-published apps**.

---

## Production App Recommendations

### Meta-Specific Metadata

Add to `<activity>` in `AndroidManifest.xml` for better Quest integration:

```xml
<activity android:name="android.app.NativeActivity" ...>

    <!-- Existing meta-data -->
    <meta-data
        android:name="android.app.lib_name"
        android:value="hello_xr" />

    <!-- Add these for Meta Quest optimization -->
    <meta-data
        android:name="com.oculus.vr.focusaware"
        android:value="true" />
    <meta-data
        android:name="com.oculus.intent.category.VR"
        android:value="vr_only" />

    <!-- Existing intent-filter -->
    <intent-filter>
        <action android:name="android.intent.action.MAIN" />
        <category android:name="android.intent.category.LAUNCHER" />
        <category android:name="org.khronos.openxr.intent.category.IMMERSIVE_HMD" />
    </intent-filter>
</activity>
```

### Release Build Signing

1. Create a release keystore:
   ```cmd
   keytool -genkey -v -keystore my-release-key.keystore -alias my-key-alias -keyalg RSA -keysize 2048 -validity 10000
   ```

2. Create `keystore.properties`:
   ```properties
   keyAlias=my-key-alias
   keyPassword=your_key_password
   storeFile=my-release-key.keystore
   storePassword=your_store_password
   ```

3. Set environment variable:
   ```cmd
   set KEYSTORE_PROPERTIES=C:\path\to\keystore.properties
   ```

4. Build signed release:
   ```cmd
   gradlew.bat assembleVulkanRelease
   ```

### Quest Store Distribution

- Store-published apps have proper lifecycle management
- Icons install/uninstall correctly (no ghost icons)
- Required for public distribution
- Follow Meta's submission guidelines at developer.oculus.com

---

## Debugging

### Android Studio (Optional)

**When to use:**
- Setting breakpoints in initialization code
- Stepping through C++ native code
- Profiling memory/CPU usage with built-in tools
- Inspecting variables during crashes

**When NOT needed:**
- Standard build/deploy/test iterations (use CLI + MQDH)
- Viewing logs (use ADB logcat or MQDH)
- Performance profiling (use MQDH GPU metrics)

**Setup:**
1. Open hello_xr directory in Android Studio
2. Select Build Variant: `vulkanDebug`
3. Connect Quest via USB
4. Click Run (▶️) or Debug (🐛)

**Note**: VR apps must be launched while wearing headset for OpenXR runtime to initialize.

### Native C++ Debugging

For breakpoints in native code (hello_xr.cpp):
1. Use Android Studio with "Debug" configuration
2. Set breakpoints in C++ files
3. Attach debugger before VR runtime starts
4. Breakpoints in render loop are impractical (breaks VR experience)

### Log-Based Debugging (Recommended)

Add logging to C++ code:
```cpp
#include <android/log.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "hello_xr", __VA_ARGS__)

LOGD("Frame %d, Position: (%.2f, %.2f, %.2f)", frameNum, x, y, z);
```

View in real-time:
```cmd
adb logcat -s "hello_xr"
```

---

## Toolset Summary

### Core Tools (Required)

| Tool | Purpose | When to Use |
|------|---------|-------------|
| **Gradle (gradlew.bat)** | Build, install | Every build iteration |
| **ADB** | Deploy, launch, logs | Command-line workflow |
| **MQDH** | Screen casting, profiling | Visual debugging, performance |
| **VSCode/Visual Studio** | C++ code editing | Writing code |
| **CMake** | Native build config | Modifying C++ build |

### Optional Tools

| Tool | Purpose | When to Use |
|------|---------|-------------|
| **Android Studio** | Visual debugging | Crash investigation only |
| **scrcpy** | Lightweight casting | Alternative to MQDH casting |
| **Meta Quest Mobile App** | Enable developer mode | Initial setup |

### For Native NDK+OpenXR Development

**✅ Essential:**
- CLI (Gradle + ADB)
- MQDH (casting + profiling)
- Code editor (VSCode/Visual Studio)

**❌ Not needed:**
- Unity
- Unreal Engine
- Heavy IDE usage (Android Studio is optional)

---

## Common Issues

### Build fails with "aapt2 POM parse error"

**Cause**: AGP 8.13.1 doesn't exist (only 8.13.0 is released)

**Fix**: Edit `build.gradle`:
```gradle
plugins {
    id 'com.android.application' version '8.13.0'  // Not 8.13.1
}
```

Then clear Gradle cache:
```cmd
gradlew.bat clean
```

### App not launching via ADB

**Symptoms**: "Process not found" error

**Cause**: VR apps require OpenXR runtime active (headset worn)

**Fix**:
- Put on headset before launching
- Or launch via MQDH "Launch App" button
- Check logs: `adb logcat | findstr FATAL`

### App icon not appearing in Quest

**Expected behavior**: Pure OpenXR apps don't always show launcher icons

**Workaround**:
- Launch via MQDH "Launch App"
- Or use ADB: `adb shell monkey -p org.khronos.openxr.hello_xr.vulkan 1`
- Icon appears in "Unknown Sources" after first launch

### MQDH not showing cast/record icons

**Cause**: Device not properly connected or authorized

**Fix**:
1. Check `adb devices` shows device (not "unauthorized")
2. Approve "Allow USB debugging" prompt in headset
3. Click refresh (🔄) in MQDH
4. Reconnect USB cable

---

## Performance Optimization

### Enable GPU Profiling

In MQDH Device Actions:
- Toggle "GPU Profiling Service" → On
- Install "OVR Metrics Tool"
- Metrics overlay appears in VR

### Monitor Key Metrics

```cmd
:: Surface Flinger stats
adb shell dumpsys SurfaceFlinger

:: GPU stats
adb shell dumpsys gpu

:: Memory usage
adb shell dumpsys meminfo org.khronos.openxr.hello_xr.vulkan
```

### Vulkan Validation Layers

Enable in manifest or via runtime flags, monitor with:
```cmd
adb logcat -s "VALIDATION"
```

---

## Quick Reference Card

```cmd
# Build & Install
gradlew.bat installVulkanDebug

# Launch
adb shell monkey -p org.khronos.openxr.hello_xr.vulkan 1

# Logs
adb logcat -c && adb logcat | findstr hello_xr

# Stop
adb shell am force-stop org.khronos.openxr.hello_xr.vulkan

# Uninstall
adb uninstall org.khronos.openxr.hello_xr.vulkan

# Clear ghost icons
adb shell pm clear com.oculus.vrshell && adb shell pm clear com.oculus.systemux && adb reboot

# Full one-liner
gradlew.bat installVulkanDebug && adb shell monkey -p org.khronos.openxr.hello_xr.vulkan 1 && adb logcat | findstr hello_xr
```

---

## Other Platforms

### macOS Support

✅ **Fully supported** - hello_xr builds natively on macOS

**Key differences:**
- Uses **Metal** graphics API (instead of D3D/Vulkan)
- Requires **ARKit** or compatible OpenXR runtime
- Same CMake build system as Windows
- Source code already includes `graphicsplugin_metal.cpp`

**Build on macOS:**
```bash
cd /path/to/OpenXR-SDK-Source
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
cmake --build . --config Debug
./src/tests/hello_xr/hello_xr -g Metal
```

### iOS Support

❌ **Not directly supported** in hello_xr sample

**Limitations:**
- Requires iOS-specific platform layer (similar to Android)
- Would need Xcode project configuration
- Apple Vision Pro uses **visionOS SDK** (not standard OpenXR)

**Recommendation:** For iOS/Vision Pro development, use Apple's native XR frameworks (RealityKit) rather than OpenXR.

---

## Additional Resources

- **OpenXR Specification**: https://www.khronos.org/registry/OpenXR/
- **Meta Quest Developer Center**: https://developer.oculus.com/
- **Android NDK Documentation**: https://developer.android.com/ndk
- **Vulkan Documentation**: https://www.khronos.org/vulkan/

---

**Document Version**: 1.0
**Last Updated**: 2025-12-03
**Target Platform**: Meta Quest 3 (Horizon OS v81, Android API 34)
**OpenXR Version**: 1.1.53
