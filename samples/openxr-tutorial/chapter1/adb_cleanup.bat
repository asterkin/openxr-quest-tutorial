@echo off
echo ========================================
echo ADB Cleanup for Quest 3 Device
echo OpenXR Tutorial Chapter 1
echo ========================================

echo Step 1: Clearing logcat buffer...
adb logcat -c
if %errorlevel% neq 0 (
    echo Warning: Failed to clear logcat buffer
    echo This may indicate ADB connection issues
)

echo.
echo Step 2: Force-stopping OpenXR Tutorial Ch1...
adb shell am force-stop com.example.openxr_tutorial_ch1
if %errorlevel% equ 0 (
    echo Successfully force-stopped OpenXR Tutorial Ch1
) else (
    echo App not running or force-stop failed
)

echo.
echo Step 3: Uninstalling OpenXR Tutorial Ch1...
adb uninstall com.example.openxr_tutorial_ch1
if %errorlevel% equ 0 (
    echo Successfully uninstalled OpenXR Tutorial Ch1
) else (
    echo No previous version found or uninstall failed
    echo This is normal for first-time installations
)

echo.
echo Step 4: Clearing package data/cache...
adb shell pm clear com.example.openxr_tutorial_ch1
if %errorlevel% neq 0 (
    echo Note: Package not found (expected after uninstall)
)

echo.
echo ADB cleanup completed - Quest 3 ready for clean installation
echo.
echo Next step: Build and install with gradlew assembleVulkanDebug
adb shell am force-stop com.oculus.vrshell
adb shell monkey -p com.oculus.vrshell 1
exit /b 0
