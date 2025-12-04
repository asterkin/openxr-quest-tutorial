# Developer Guidelines for OpenXR hello_xr on Windows

> **Companion guide for native Windows development**
> See [Developers_Guidelines.md](Developers_Guidelines.md) for Quest 3 Android deployment

---

## Overview

The OpenXR SDK hello_xr sample is **fully cross-platform** and can be built natively for Windows as a Win32 desktop application. This enables rapid development, better debugging, and testing with PC VR headsets including Quest via Link.

---

## Why Build for Windows?

### Advantages Over Quest-Only Development

| Benefit | Description |
|---------|-------------|
| **Faster Iteration** | Instant launch (no APK build/deploy cycle) |
| **Better Debugging** | Full Visual Studio debugger with native breakpoints |
| **Superior Profiling** | NSight Graphics, RenderDoc, PIX, Visual Studio Profiler |
| **No Headset Required** | Can run in mock runtime mode for logic testing |
| **Cross-Platform Validation** | Ensures code works on multiple runtimes |
| **PC VR Testing** | Test Quest Link, SteamVR, Windows Mixed Reality |

### When to Use Windows Build

✅ **Use Windows build for:**
- Algorithm and logic development
- Initial OpenXR API integration
- Performance profiling with advanced tools
- Testing cross-platform compatibility
- Quest Link PC VR streaming validation

❌ **Must use Quest build for:**
- Quest-specific features (passthrough, guardian boundaries)
- Android-specific APIs (JNI, Android services)
- Mobile GPU performance validation
- Final deployment testing

---

## Prerequisites

### Required Software

1. **Visual Studio 2022** (Community Edition or higher)
   - Install "Desktop development with C++" workload
   - Include Windows 10/11 SDK
   - Download: https://visualstudio.microsoft.com/

2. **CMake 3.20 or later**
   - Download: https://cmake.org/download/
   - Add to PATH during installation

3. **Vulkan SDK** (optional, for Vulkan graphics plugin)
   - Download: https://vulkan.lunarg.com/
   - Required only if using `-g Vulkan` option

4. **OpenXR Runtime** (one of):
   - **Meta Quest Link** (for Quest as PC VR headset)
   - **SteamVR** (for Valve Index, HTC Vive)
   - **Windows Mixed Reality** (for WMR headsets)
   - **OculusVR** (for Rift/Rift S)

### Optional Tools

- **RenderDoc** - Graphics debugging (https://renderdoc.org/)
- **NVIDIA NSight Graphics** - GPU profiling (NVIDIA GPUs only)
- **PIX** - DirectX debugging (Microsoft)
- **Visual Studio Graphics Debugger** - Built into VS

---

## Build Instructions

### Quick Start (Command Line)

```cmd
:: 1. Navigate to OpenXR SDK root
cd c:\Users\PRO\Projects\OpenXR-SDK-Source

:: 2. Create and enter build directory
mkdir build
cd build

:: 3. Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON

:: 4. Build Debug configuration
cmake --build . --config Debug

:: 5. Build Release configuration
cmake --build . --config Release

:: 6. Run the application
.\src\tests\hello_xr\Debug\hello_xr.exe --help
```

### CMake Configuration Options

#### Minimal Build (hello_xr only)

```cmd
cmake .. -G "Visual Studio 17 2022" -A x64 ^
  -DBUILD_TESTS=ON ^
  -DBUILD_LOADER=ON ^
  -DBUILD_API_LAYERS=OFF ^
  -DBUILD_CONFORMANCE_TESTS=OFF
```

#### Full Build (with validation layers)

```cmd
cmake .. -G "Visual Studio 17 2022" -A x64 ^
  -DBUILD_TESTS=ON ^
  -DBUILD_LOADER=ON ^
  -DBUILD_API_LAYERS=ON ^
  -DBUILD_ALL_EXTENSIONS=ON ^
  -DBUILD_CONFORMANCE_TESTS=OFF
```

#### Specify Graphics APIs

```cmd
cmake .. -G "Visual Studio 17 2022" -A x64 ^
  -DBUILD_TESTS=ON ^
  -DXR_USE_GRAPHICS_API_VULKAN=ON ^
  -DXR_USE_GRAPHICS_API_D3D11=ON ^
  -DXR_USE_GRAPHICS_API_D3D12=ON ^
  -DXR_USE_GRAPHICS_API_OPENGL=ON
```

**Note**: By default, all available graphics APIs are enabled. Disable unwanted ones with `=OFF`.

---

## Running hello_xr on Windows

### Command Line Options

```cmd
:: Display help
hello_xr.exe --help
hello_xr.exe -h

:: Specify graphics plugin
hello_xr.exe -g D3D11        # Direct3D 11 (default on Windows)
hello_xr.exe -g D3D12        # Direct3D 12
hello_xr.exe -g Vulkan       # Vulkan (requires Vulkan SDK)
hello_xr.exe -g OpenGL       # OpenGL

:: Specify form factor
hello_xr.exe -ff Hmd         # Head-Mounted Display (default)
hello_xr.exe -ff Handheld    # Handheld device

:: Specify view configuration
hello_xr.exe -vc Stereo      # Stereo rendering (default)
hello_xr.exe -vc Mono        # Mono rendering

:: Specify blend mode
hello_xr.exe -bm Opaque      # VR (default)
hello_xr.exe -bm Additive    # AR additive
hello_xr.exe -bm AlphaBlend  # AR alpha blend

:: Combine options
hello_xr.exe -g Vulkan -ff Hmd -vc Stereo -bm Opaque
```

### Graphics Plugin Comparison

| API | Performance | Compatibility | Debugging Tools | Recommended For |
|-----|-------------|---------------|-----------------|-----------------|
| **D3D11** | Good | Excellent | PIX, VS Graphics Debugger | General development |
| **D3D12** | Excellent | Good (Win10+) | PIX, NSight | Performance optimization |
| **Vulkan** | Excellent | Good (requires SDK) | RenderDoc, NSight | Cross-platform testing |
| **OpenGL** | Good | Excellent | RenderDoc, apitrace | Legacy compatibility |

---

## VR Runtime Setup

### Option 1: Meta Quest Link (Recommended for Quest Developers)

**Use Case**: Test Quest as a PC VR headset via USB or Wi-Fi

1. **Install Meta Quest Link Software**
   - Download from: https://www.meta.com/quest/setup/
   - Install on Windows PC

2. **Connect Quest to PC**
   - **USB Method**: Use USB-C cable (USB 3.0+ recommended)
   - **Wireless Method**: Enable Air Link in Quest settings

3. **Enable Link in Quest**
   - Put on Quest headset
   - Accept "Allow USB debugging" prompt
   - Click "Enable Link" when prompted

4. **Verify OpenXR Runtime**
   ```cmd
   :: Check active OpenXR runtime (should show Oculus)
   reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\OpenXR\1" /v ActiveRuntime
   ```

5. **Run hello_xr**
   ```cmd
   hello_xr.exe -g D3D11
   ```

### Option 2: SteamVR (For Steam-Compatible Headsets)

**Use Case**: Valve Index, HTC Vive, or Quest via Steam Link

1. **Install SteamVR**
   - Install Steam client
   - Install SteamVR from Steam Library

2. **Set as OpenXR Runtime**
   - Open SteamVR settings
   - Navigate to Developer tab
   - Click "Set SteamVR as OpenXR Runtime"

3. **Run hello_xr**
   ```cmd
   hello_xr.exe -g Vulkan
   ```

### Option 3: Windows Mixed Reality

**Use Case**: HP Reverb G2, Samsung Odyssey, other WMR headsets

1. **Enable Windows Mixed Reality**
   - Windows Settings → Mixed Reality → Set up Mixed Reality

2. **Install Mixed Reality Portal**
   - Pre-installed on Windows 10/11
   - Or download from Microsoft Store

3. **Run hello_xr**
   ```cmd
   hello_xr.exe -g D3D11
   ```

### Runtime Switching

```cmd
:: View current OpenXR runtime
reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\OpenXR\1" /v ActiveRuntime

:: Manually set runtime (requires admin)
:: Oculus:
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\OpenXR\1" /v ActiveRuntime /d "C:\Program Files\Oculus\Support\oculus-runtime\oculus_openxr_64.json" /f

:: SteamVR:
reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\OpenXR\1" /v ActiveRuntime /d "C:\Program Files (x86)\Steam\steamapps\common\SteamVR\steamxr_win64.json" /f
```

**Tip**: Use Meta/Steam runtime switcher tools instead of manual registry edits.

---

## Debugging in Visual Studio

### Opening the Project

```cmd
:: Option 1: Open generated .sln file
cd c:\Users\PRO\Projects\OpenXR-SDK-Source\build
start OPENXR.sln

:: Option 2: Use CMake GUI
cmake-gui ..
:: Configure → Generate → Open Project
```

### Setting hello_xr as Startup Project

1. In Visual Studio Solution Explorer, right-click `hello_xr`
2. Select **"Set as Startup Project"**
3. Right-click again → **Properties** → **Debugging**
4. Set **Command Arguments**: `-g D3D11` (or preferred graphics API)

### Debugging Workflow

#### Setting Breakpoints

1. Open `main.cpp`, `openxr_program.cpp`, or graphics plugin files
2. Click left margin to set breakpoints (red dots)
3. Press **F5** to start debugging
4. Put on VR headset when app launches

#### Common Breakpoint Locations

```cpp
// main.cpp - Application entry
int WINAPI WinMain(HINSTANCE hInstance, ...)

// openxr_program.cpp - OpenXR initialization
void OpenXrProgram::CreateInstance()
void OpenXrProgram::InitializeSession()

// openxr_program.cpp - Main render loop
void OpenXrProgram::RenderFrame()

// graphicsplugin_*.cpp - Graphics rendering
void RenderView(...)
```

#### Watch Variables

Common variables to inspect:
- `m_instance` - OpenXR instance handle
- `m_session` - OpenXR session handle
- `m_viewConfigType` - View configuration (stereo/mono)
- `m_views` - View transforms and FOV
- `m_swapchainImages` - Swapchain image array

#### Call Stack Analysis

When debugging crashes:
1. Break on exception (**Debug → Windows → Exception Settings** → check C++ Exceptions)
2. View **Call Stack** window (**Debug → Windows → Call Stack**)
3. Double-click frames to navigate code

---

## Performance Profiling

### Visual Studio Profiler (Built-in)

```cmd
:: 1. Build Release configuration
cmake --build . --config Release

:: 2. In Visual Studio: Debug → Performance Profiler (Alt+F2)
:: 3. Select: CPU Usage, GPU Usage, Memory Usage
:: 4. Click "Start" and wear headset
:: 5. Run application for 30-60 seconds
:: 6. Stop profiler to view results
```

### RenderDoc (Graphics Debugging)

**Best for**: Frame capture, shader debugging, draw call analysis

1. **Launch RenderDoc**
2. **Executable Path**: `C:\...\build\src\tests\hello_xr\Release\hello_xr.exe`
3. **Command Line Args**: `-g Vulkan` or `-g D3D11`
4. **Click "Launch"**
5. **Press F12** in VR to capture frame
6. Analyze:
   - Texture viewer (see render targets)
   - Pipeline state (shader inputs/outputs)
   - Event browser (draw calls)
   - Mesh viewer (geometry)

### NVIDIA NSight Graphics (NVIDIA GPUs)

**Best for**: GPU performance analysis, shader optimization

```cmd
:: 1. Install NSight Graphics from NVIDIA website
:: 2. Launch NSight Graphics
:: 3. File → Connect → Local application
:: 4. Select hello_xr.exe
:: 5. Add arguments: -g D3D12 (or Vulkan)
:: 6. Click "Launch"
:: 7. Press Space to capture frame
```

**Key Metrics**:
- GPU utilization
- Bottleneck analysis (vertex/pixel/memory bound)
- Shader execution time
- Memory bandwidth usage

### PIX (DirectX Debugging)

**Best for**: DirectX 11/12 debugging on Windows

1. **Install PIX** from Microsoft website
2. **Select GPU Capture**
3. **Target Application**: `hello_xr.exe`
4. **Arguments**: `-g D3D12`
5. **Launch and Capture**
6. View:
   - Timeline (GPU execution)
   - Event list (draw calls)
   - Resource usage
   - Shader debugging

---

## Headless Testing (No VR Hardware)

### Mock Runtime

For unit testing OpenXR logic without VR hardware:

```cmd
:: Build with mock runtime support
cmake .. -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON

:: Set environment variable
set XR_RUNTIME_JSON=%CD%\src\tests\test_runtimes\mock_runtime.json

:: Run hello_xr (will use mock runtime)
hello_xr.exe -g D3D11
```

**Limitations**:
- No actual rendering (black screen)
- Simulated pose data
- Useful for API call validation only

### Validation Layer

Enable OpenXR validation for development:

```cmd
:: Build with API layers
cmake .. -G "Visual Studio 17 2022" -A x64 -DBUILD_API_LAYERS=ON

:: Enable validation layer
set XR_ENABLE_API_LAYERS=XR_APILAYER_LUNARG_core_validation

:: Run hello_xr
hello_xr.exe -g D3D11
```

Validation output appears in **Output → Debug** window in Visual Studio.

---

## Common Build Issues

### Issue: CMake can't find Visual Studio

**Error**: `Could not find Visual Studio compiler`

**Fix**:
```cmd
:: Explicitly specify generator
cmake .. -G "Visual Studio 17 2022" -A x64

:: If VS 2019:
cmake .. -G "Visual Studio 16 2019" -A x64
```

### Issue: Vulkan SDK not found

**Error**: `Could NOT find Vulkan`

**Fix**:
1. Install Vulkan SDK from https://vulkan.lunarg.com/
2. Set environment variable:
   ```cmd
   set VULKAN_SDK=C:\VulkanSDK\1.3.XXX.X
   ```
3. Re-run CMake

Or disable Vulkan:
```cmd
cmake .. -G "Visual Studio 17 2022" -A x64 -DXR_USE_GRAPHICS_API_VULKAN=OFF
```

### Issue: OpenXR loader not found

**Error**: Runtime linking error when launching

**Fix**:
```cmd
:: Ensure loader is built
cmake --build . --config Debug --target openxr_loader

:: Verify DLL exists
dir src\loader\Debug\openxr_loader.dll
```

### Issue: No OpenXR runtime detected

**Error**: `XR_ERROR_RUNTIME_UNAVAILABLE`

**Fix**:
1. Install a runtime (Meta Quest Link, SteamVR, WMR)
2. Verify registry:
   ```cmd
   reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\OpenXR\1" /v ActiveRuntime
   ```
3. If empty, launch runtime software (Oculus app, SteamVR) before running hello_xr

---

## Comparison: Windows vs Quest Development

| Aspect | Windows (PC VR) | Quest (Native) |
|--------|-----------------|----------------|
| **Build System** | CMake + Visual Studio | Gradle + Android NDK |
| **Build Time** | ~30 seconds (incremental) | ~2 minutes (full rebuild) |
| **Deploy Time** | Instant (local .exe) | 10-30 seconds (ADB install) |
| **Launch Time** | <1 second | 2-5 seconds |
| **Debugging** | Full Visual Studio debugger | ADB logcat + limited breakpoints |
| **Breakpoints** | All code paths | Initialization only (VR disrupts) |
| **Profiling** | NSight, RenderDoc, PIX, VS Profiler | MQDH GPU profiler only |
| **Graphics APIs** | D3D11, D3D12, OpenGL, Vulkan | OpenGL ES 3.x, Vulkan 1.0 |
| **Shaders** | HLSL (D3D), GLSL (GL/VK) | GLSL ES, SPIR-V (Vulkan) |
| **Hot Reload** | Fast (F5 restart) | Slow (rebuild + reinstall) |
| **Runtime** | Oculus, SteamVR, WMR | Meta XR (native Quest) |
| **CPU** | x64 desktop CPU | ARM Snapdragon XR2 Gen 2 |
| **GPU** | Desktop GPU (NVIDIA/AMD) | Adreno 740 (mobile) |
| **Memory** | 16-32+ GB | 8-12 GB unified |
| **Performance Target** | 90 FPS (headset dependent) | 72/90/120 FPS (Quest modes) |

---

## Recommended Workflows

### Workflow 1: Algorithm Development

```
1. Write C++ code in Visual Studio (Windows)
2. Build and debug on Windows with breakpoints
3. Test logic without VR hardware (mock runtime)
4. Validate with Quest Link (see VR output)
5. Deploy to Quest for final validation
```

### Workflow 2: Cross-Platform Graphics

```
1. Develop shaders/rendering on Windows (RenderDoc)
2. Test with Vulkan (cross-platform API)
3. Profile with NSight Graphics
4. Deploy to Quest with same Vulkan code
5. Optimize for mobile GPU (MQDH profiler)
```

### Workflow 3: Quest Link Testing

```
1. Build Windows .exe
2. Connect Quest via Link cable/Air Link
3. Run hello_xr.exe (renders to Quest)
4. Debug on PC while viewing in Quest
5. Profile with PC tools while running on Quest hardware
```

### Workflow 4: Performance Optimization

```
1. Profile on Windows (identify bottlenecks)
2. Optimize shaders with RenderDoc/NSight
3. Test CPU performance with VS Profiler
4. Deploy to Quest (validate mobile performance)
5. Use MQDH for Quest-specific profiling
```

---

## Development Best Practices

### Code Organization

```cpp
// Use platform-agnostic code
#ifdef XR_USE_PLATFORM_WIN32
    // Windows-specific (Win32 window creation)
#endif

#ifdef XR_USE_PLATFORM_ANDROID
    // Android-specific (JNI, NativeActivity)
#endif

// Keep graphics code in plugins (already done in hello_xr)
// graphicsplugin_d3d11.cpp  - Windows only
// graphicsplugin_vulkan.cpp - Cross-platform
// graphicsplugin_opengles.cpp - Android/mobile
```

### Graphics API Strategy

**For Maximum Portability**:
1. Use **Vulkan** on both Windows and Quest
2. Share shader code (GLSL → SPIR-V)
3. Test on Windows, deploy to Quest unchanged
4. Minor adjustments for mobile GPU limits

**For Platform-Specific Optimization**:
1. Use **D3D11/D3D12** on Windows (best tools)
2. Use **Vulkan** on Quest (best performance)
3. Maintain separate graphics paths
4. Share high-level rendering logic

---

## Build Automation

### PowerShell Build Script

Save as `build-windows.ps1`:

```powershell
# Build hello_xr for Windows
param(
    [ValidateSet('Debug', 'Release')]
    [string]$Config = 'Debug'
)

$ProjectRoot = "c:\Users\PRO\Projects\OpenXR-SDK-Source"
$BuildDir = "$ProjectRoot\build"

# Create build directory
if (!(Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

# Configure
Write-Host "Configuring CMake..." -ForegroundColor Cyan
Set-Location $BuildDir
cmake .. -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# Build
Write-Host "Building $Config configuration..." -ForegroundColor Green
cmake --build . --config $Config
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

# Success
$ExePath = "$BuildDir\src\tests\hello_xr\$Config\hello_xr.exe"
Write-Host "Build complete: $ExePath" -ForegroundColor Green
```

### Usage

```cmd
:: Debug build
powershell .\build-windows.ps1 -Config Debug

:: Release build
powershell .\build-windows.ps1 -Config Release
```

---

## Integration with Quest Development

### Shared Codebase Strategy

```
OpenXR-SDK-Source/
├── src/tests/hello_xr/           # Shared C++ code
│   ├── main.cpp                  # Cross-platform entry point
│   ├── openxr_program.cpp        # OpenXR logic (shared)
│   ├── graphicsplugin_vulkan.cpp # Vulkan (Windows + Quest)
│   ├── graphicsplugin_d3d11.cpp  # D3D11 (Windows only)
│   ├── graphicsplugin_opengles.cpp # GLES (Android only)
│   ├── platformplugin_win32.cpp  # Windows platform layer
│   └── platformplugin_android.cpp # Android platform layer
│
├── build/                        # Windows CMake build output
│   └── src/tests/hello_xr/
│       └── Debug/hello_xr.exe    # Windows executable
│
└── src/tests/hello_xr/           # Android Gradle project
    ├── build.gradle              # Android build config
    └── build/outputs/apk/        # Android APK output
```

### Workflow: Develop on Windows, Deploy to Quest

1. **Develop on Windows**:
   ```cmd
   cd c:\Users\PRO\Projects\OpenXR-SDK-Source\build
   cmake --build . --config Debug
   .\src\tests\hello_xr\Debug\hello_xr.exe -g Vulkan
   ```

2. **Test with Quest Link** (same PC build, Quest display)

3. **Build for Quest** (when ready):
   ```cmd
   cd c:\Users\PRO\Projects\OpenXR-SDK-Source\src\tests\hello_xr
   gradlew.bat installVulkanDebug
   ```

4. **Deploy to Quest**:
   ```cmd
   adb shell monkey -p org.khronos.openxr.hello_xr.vulkan 1
   ```

---

## Quick Reference Card (Windows)

```cmd
# CMake Configure
cmake .. -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTS=ON

# Build Debug
cmake --build . --config Debug

# Build Release
cmake --build . --config Release

# Run with different graphics APIs
hello_xr.exe -g D3D11   # DirectX 11 (default)
hello_xr.exe -g D3D12   # DirectX 12
hello_xr.exe -g Vulkan  # Vulkan
hello_xr.exe -g OpenGL  # OpenGL

# Check OpenXR runtime
reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\OpenXR\1" /v ActiveRuntime

# Enable validation
set XR_ENABLE_API_LAYERS=XR_APILAYER_LUNARG_core_validation

# Full rebuild
cmake --build . --config Debug --clean-first

# Open in Visual Studio
start OPENXR.sln
```

---

## Additional Resources

- **OpenXR Specification**: https://www.khronos.org/registry/OpenXR/
- **OpenXR SDK GitHub**: https://github.com/KhronosGroup/OpenXR-SDK
- **Meta Quest Link Setup**: https://www.meta.com/quest/setup/
- **SteamVR OpenXR**: https://store.steampowered.com/app/1059530/SteamVR/
- **Visual Studio Docs**: https://docs.microsoft.com/en-us/visualstudio/
- **CMake Documentation**: https://cmake.org/documentation/
- **RenderDoc**: https://renderdoc.org/docs/
- **NVIDIA NSight**: https://developer.nvidia.com/nsight-graphics

---

**Document Version**: 1.0
**Last Updated**: 2025-12-03
**Target Platform**: Windows 10/11 (x64)
**OpenXR Version**: 1.1.53
**Related**: [Developers_Guidelines.md](Developers_Guidelines.md) (Quest 3 Android)
