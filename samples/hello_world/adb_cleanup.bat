@echo off
echo ========================================
echo ADB Cleanup for Quest 3 Device
echo hello_world OpenXR sample
echo ========================================

echo Step 1: Clearing logcat buffer...
adb logcat -c
if %errorlevel% neq 0 (
    echo Warning: Failed to clear logcat buffer
    echo This may indicate ADB connection issues
)

echo.
echo Step 2: Force-stopping hello_world...
adb shell am force-stop com.example.hello_world
if %errorlevel% equ 0 (
    echo Successfully force-stopped hello_world
) else (
    echo App not running or force-stop failed
)

echo.
echo Step 3: Uninstalling hello_world...
adb uninstall com.example.hello_world
if %errorlevel% equ 0 (
    echo Successfully uninstalled hello_world
) else (
    echo No previous version found or uninstall failed
    echo This is normal for first-time installations
)

echo.
echo Step 4: Clearing package data/cache...
adb shell pm clear com.example.hello_world
if %errorlevel% neq 0 (
    echo Note: Package not found (expected after uninstall)
)

echo.
echo ADB cleanup completed - Quest 3 ready for clean hello_world installation
echo.
echo Next step: Build and install with gradlew assembleDebug
exit /b 0
