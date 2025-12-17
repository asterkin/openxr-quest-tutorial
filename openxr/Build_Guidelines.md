# OpenXR Samples Build Guidelines

This document provides common instructions for building, deploying, and debugging OpenXR samples on Meta Quest.

> **Architecture Reference**: For build system design decisions, see [ADR-0007: Build and Run Architecture](../docs/adrs/adr-0007-build-and-run-architecture.md).

---

## Prerequisites

- Quest 3 connected via USB with Developer Mode enabled
- ADB installed and device authorized (`adb devices` shows your device)
- Environment configured per [Environment_Setup.md](../docs/Environment_Setup.md)

---

## Standard Build Workflow

All commands are run from within a sample's directory (e.g., `samples/hello_world/` or `samples/openxr-tutorial/Chapter1/`).

### 1. Clean the Project

Before starting a fresh build, clean previous artifacts:

```powershell
.\gradlew.bat clean
```

For stubborn build issues, manually delete cache directories:
```powershell
Remove-Item -Recurse -Force .cxx, build, app\build -ErrorAction SilentlyContinue
```

### 2. Build and Install

Build the APK and install directly to connected Quest:

```powershell
# Standard samples (hello_world, hello_xr single-flavor)
.\gradlew.bat installDebug

# hello_xr with product flavors
.\gradlew.bat installVulkanDebug      # Vulkan backend (recommended)
.\gradlew.bat installOpenGLESDebug    # OpenGL ES backend
```

**First Run Note**: The app appears in Quest's **"Unknown Sources"** section of the App Library.

### 3. Build Only (No Install)

To build the APK without installing:

```powershell
.\gradlew.bat assembleDebug
# Output: build/outputs/apk/debug/*.apk
```

### 4. Uninstall

```powershell
.\gradlew.bat uninstallDebug

# Or with flavors
.\gradlew.bat uninstallVulkanDebug
```

---

## ADB Device Operations

### Connection Verification

```powershell
# List connected devices
adb devices

# Expected output:
# List of devices attached
# 1WMHH8xxxxxx    device
```

### Application Launch

Launch an app without wearing the headset (useful for log verification):

```powershell
# Generic pattern
adb shell am start -n <package>/<activity>

# hello_world
adb shell am start -n com.example.openxr.helloworld/android.app.NativeActivity

# hello_xr (Vulkan)
adb shell am start -n org.khronos.openxr.hello_xr.vulkan/android.app.NativeActivity

# hello_xr (OpenGL ES)
adb shell am start -n org.khronos.openxr.hello_xr.opengles/android.app.NativeActivity

# openxr-tutorial Chapter1
adb shell am start -n com.example.openxrtutorialchapter1/android.app.NativeActivity
```

### Application Stop

```powershell
# Force stop an app
adb shell am force-stop <package>

# Examples
adb shell am force-stop com.example.openxr.helloworld
adb shell am force-stop org.khronos.openxr.hello_xr.vulkan
```

### Application Uninstall (via ADB)

```powershell
# Uninstall by package name
adb uninstall <package>

# Examples
adb uninstall com.example.openxr.helloworld
adb uninstall org.khronos.openxr.hello_xr.vulkan
```

### Clear App Data

```powershell
adb shell pm clear <package>
```

---

## Logging and Debugging

### Real-time Log Streaming

```powershell
# All logs (verbose)
adb logcat

# Clear buffer and stream
adb logcat -c && adb logcat

# Filter by tag (sample-specific)
adb logcat -s "OpenXR_HelloWorld:*"     # hello_world
adb logcat -s "hello_xr:*"              # hello_xr
adb logcat -s "OpenXRTutorial:*"        # openxr-tutorial chapters

# Filter OpenXR and errors only
adb logcat -s OpenXR:* *:E

# Save to file
adb logcat -d > logcat_output.txt
```

### Common Log Tags

| Sample | Primary Tag | Additional Tags |
|--------|-------------|-----------------|
| hello_world | `OpenXR_HelloWorld` | - |
| hello_xr | `hello_xr` | `OpenXR`, `Vulkan` |
| openxr-tutorial | `OpenXRTutorial` | `GraphicsAPI`, `OpenXR` |

### Crash Investigation

```powershell
# Dump crash logs (last crash)
adb logcat -b crash -d

# Native crash stack traces
adb logcat -s DEBUG:*

# Check for ANR (Application Not Responding)
adb logcat -s ActivityManager:* | findstr ANR
```

---

## Quest-Specific Operations

### Restart VR Shell (Clear Stale Icons)

After uninstalling apps, Quest may retain stale icons. Restart vrshell to clear:

```powershell
adb shell am force-stop com.oculus.vrshell
adb shell monkey -p com.oculus.vrshell 1
```

**Note**: This is automatically performed by Gradle's `uninstall` tasks via a post-uninstall hook.

### Check Installed Packages

```powershell
# List all packages
adb shell pm list packages

# Filter by keyword
adb shell pm list packages | findstr openxr
adb shell pm list packages | findstr hello
```

### Device Information

```powershell
# Device model and OS version
adb shell getprop ro.product.model
adb shell getprop ro.build.version.release

# Quest-specific info
adb shell getprop ro.build.fingerprint
```

---

## Troubleshooting

### Build Fails or Freezes

```powershell
# Stop Gradle daemon
.\gradlew.bat --stop

# Clean and retry
.\gradlew.bat clean
.\gradlew.bat installDebug
```

### CMake Cache Issues

After changing CMakeLists.txt significantly:

```powershell
# Delete CMake cache
Remove-Item -Recurse -Force .cxx -ErrorAction SilentlyContinue
.\gradlew.bat installDebug
```

### "Unknown Sources" Not Showing App

1. Ensure Developer Mode is enabled on Quest
2. Check USB connection: `adb devices`
3. Reinstall: `.\gradlew.bat uninstallDebug && .\gradlew.bat installDebug`
4. Restart vrshell (see above)

### Runtime Crash: `ANativeActivity_onCreate` Undefined

Ensure CMakeLists.txt includes:
```cmake
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-u ANativeActivity_onCreate"
)
```

### OpenXR Runtime Not Found

Ensure AndroidManifest.xml includes OpenXR permissions:
```xml
<uses-permission android:name="org.khronos.openxr.permission.OPENXR" />
<uses-permission android:name="org.khronos.openxr.permission.OPENXR_SYSTEM" />

<queries>
    <provider android:authorities="org.khronos.openxr.runtime_broker;org.khronos.openxr.system_runtime_broker" />
    <intent>
        <action android:name="org.khronos.openxr.OpenXRRuntimeService" />
    </intent>
</queries>
```

### Exiting Applications on Quest

To quit a running VR application:
1. Press the **Meta button** (oval button) on either controller
2. Locate the app window in the Universal Menu
3. Click **"Quit"**

---

## Quick Reference

| Task | Command |
|------|---------|
| Build + Install | `.\gradlew.bat installDebug` |
| Build only | `.\gradlew.bat assembleDebug` |
| Clean | `.\gradlew.bat clean` |
| Uninstall | `.\gradlew.bat uninstallDebug` |
| Launch app | `adb shell am start -n <pkg>/<activity>` |
| Stop app | `adb shell am force-stop <pkg>` |
| Stream logs | `adb logcat -s <TAG>:*` |
| Clear logs | `adb logcat -c` |
| List devices | `adb devices` |
