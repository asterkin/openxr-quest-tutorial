@echo off
echo ========================================
echo ADB Cleanup for Quest 3 Device
echo hello_xr (Vulkan variant)
echo ========================================

echo Step 1: Clearing logcat buffer...
adb logcat -c
if %errorlevel% neq 0 (
    echo Warning: Failed to clear logcat buffer
    echo This may indicate ADB connection issues
)

echo.
echo Step 2: Force-stopping hello_xr...
adb shell am force-stop org.khronos.openxr.hello_xr.vulkan
if %errorlevel% equ 0 (
    echo Successfully force-stopped hello_xr
) else (
    echo App not running or force-stop failed
)

echo.
echo Step 3: Uninstalling hello_xr (Vulkan)...
adb uninstall org.khronos.openxr.hello_xr.vulkan
if %errorlevel% equ 0 (
    echo Successfully uninstalled hello_xr (Vulkan)
) else (
    echo No previous version found or uninstall failed
    echo This is normal for first-time installations
)

echo.
echo Step 4: Uninstalling hello_xr (OpenGL ES) if present...
adb uninstall org.khronos.openxr.hello_xr.opengles
if %errorlevel% equ 0 (
    echo Successfully uninstalled hello_xr (OpenGL ES)
) else (
    echo No OpenGL ES version found (expected)
)

echo.
echo Step 5: Clearing package data/cache...
adb shell pm clear org.khronos.openxr.hello_xr.vulkan
if %errorlevel% neq 0 (
    echo Note: Package not found (expected after uninstall)
)

echo.
echo ADB cleanup completed - Quest 3 ready for clean hello_xr installation
echo.
echo Next step: Build and install with gradlew assembleVulkanDebug
exit /b 0
