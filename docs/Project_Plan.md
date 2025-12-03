# OpenXR Quest Tutorial - Project Architecture & Implementation Plan

> **Repository**: `openxr-quest-tutorial`
> **Document Location**: `docs/Project_Plan.md`

---

## Document Purpose

This document outlines the complete architecture and implementation plan for the **OpenXR Quest Tutorial** repository. This is an **independent tutorial repository** containing consolidated OpenXR samples from multiple sources, focused on **Meta Quest 3 development using NDK+OpenXR**.

This is a learning and reference repository, NOT a production development environment.

---

## Core Principles

1. ✅ **Computer-Level SDKs**: OpenXR SDK, Meta XR SDK, and Android NDK installed globally (not in repo)
2. ✅ **Tutorial Repository**: Consolidated open-source samples + learning materials
3. ✅ **Documentation-Driven**: AI Agent instructions via `CLAUDE.md` at repo root, referencing `./docs/` for detailed context
4. ✅ **Separate Production**: InnoVision SDK and customer apps in their own repositories
5. ✅ **SDK Plugin Strategy**: Meta XR Core vs All-in-One SDK clarification needed

---

## Repository Architecture

### Proposed Repository Structure

```
openxr-quest-tutorial/                 # Tutorial/learning mono-repo (Quest-focused)
├── README.md                          # Human-readable overview
├── CLAUDE.md                          # Master instructions for Claude Code (repo root)
├── docs/                              # Detailed AI Agent documentation
│   ├── AI_Agent_Guide.md              # Future: Codex/Gemini/Grok instructions
│   ├── Environment_Setup.md           # SDK installation instructions
│   ├── Build_Deploy_Guide.md          # Generic build/deploy for any sample
│   └── samples/                       # Sample-specific context docs
│       ├── hello_xr_analysis.md       # Your Application_Logic.md content
│       ├── meta_samples_overview.md
│       └── ...
│
├── samples/                           # All sample projects
│   ├── hello_xr/                      # From OpenXR-SDK-Source
│   │   ├── src/                       # C++ sources
│   │   ├── CMakeLists.txt             # References system OpenXR SDK
│   │   ├── build.gradle
│   │   └── README.md                  # Build/run instructions
│   │
│   ├── meta/                          # From Meta OVR Mobile SDK
│   │   ├── _framework/                # Shared SampleXrFramework
│   │   ├── XrPassthrough/
│   │   ├── XrHandsFB/
│   │   ├── XrControllers/
│   │   └── ... (all 19 samples)
│   │
│   └── community/                     # Future: other open-source samples
│       └── (third-party samples)
│
├── scripts/                           # AI Agent automation scripts
│   ├── build.py                       # Generic: build current project
│   ├── deploy.py                      # Generic: deploy to Quest 3
│   ├── setup_env.py                   # Verify SDK installations
│   └── utils/
│
├── settings.gradle                    # Multi-project Gradle config
├── build.gradle                       # Root build config
└── .gitignore                         # Exclude build outputs
```

---

## Phase Breakdown

### **Phase 0: Computer-Level SDK Installation** (Prerequisites)

**Goal**: Document and verify global SDK installation strategy

#### A. OpenXR SDK Installation

**Installation Path**: `C:\OpenXR-SDK\` (Windows development machine)

```
C:\OpenXR-SDK\
├── include\openxr\           # Headers (openxr.h, etc.)
├── lib\
│   └── android\              # Android libraries (Quest target)
│       └── arm64-v8a\
│           └── libopenxr_loader.so
└── share\openxr\
    └── cmake\                # OpenXRConfig.cmake
```

**Note**: Windows/Mac libraries (`windows/`, `darwin/`) not needed for Quest-focused tutorial. Directory structure shown above is for Android development only.

**Environment Variables**:
```
OPENXR_SDK_ROOT=C:\OpenXR-SDK
CMAKE_PREFIX_PATH=%CMAKE_PREFIX_PATH%;C:\OpenXR-SDK
```

**CMake Usage**:
```cmake
find_package(OpenXR REQUIRED)
target_link_libraries(hello_xr PRIVATE OpenXR::openxr_loader)
```

**Build Instructions**:
- **Android (Quest)**: Must build from source (no prebuilt binaries available)
  - Source: https://github.com/KhronosGroup/OpenXR-SDK-Source
  - See: [Building OpenXR SDK for Android](#building-openxr-sdk-for-android)
- **Windows** (future): Prebuilt binaries available from releases OR build from source
  - URL: https://github.com/KhronosGroup/OpenXR-SDK/releases
- **Note**: This tutorial focuses on Quest/Android. Windows/Mac development docs are retained for reference but require building the OpenXR SDK for those platforms.

#### B. Meta XR SDK Installation

**Current SDK Packages Available**:

| SDK Package | Purpose | Use Case |
|-------------|---------|----------|
| **Meta XR Core SDK** | Native OpenXR extensions (C/C++) | Native NDK apps |
| **Meta XR SDK All-in-One UPM** | Unity Package Manager format | Unity projects only |
| **OVR OpenXR Mobile SDK** | Full native SDK + samples | Native development (current) |

**Recommended for This Tutorial**: **OVR OpenXR Mobile SDK 81.0**

**Windows Installation Path**: `C:\Meta-XR-SDK\`

```
C:\Meta-XR-SDK\
├── OpenXR\
│   ├── Include\              # Meta OpenXR extensions
│   └── Libs\
│       └── Android\
│           └── arm64-v8a\
├── SampleXrFramework\        # Shared framework for samples
└── Samples\                  # (Reference only - not used directly)
```

**Environment Variables**:
```
META_XR_SDK_ROOT=C:\Meta-XR-SDK
```

**CMake Usage**:
```cmake
# Find Meta extensions
include_directories(${META_XR_SDK_ROOT}/OpenXR/Include)
link_directories(${META_XR_SDK_ROOT}/OpenXR/Libs/Android/${ANDROID_ABI})
```

**Download URL**: https://developers.meta.com/horizon/downloads/package/oculus-openxr-mobile-sdk

#### C. Android NDK Installation

**Installation**: Via Android Studio SDK Manager

**Expected Path**: `C:\Users\PRO\AppData\Local\Android\Sdk\ndk\27.x.x`

**Environment Variables**:
```
ANDROID_NDK_ROOT=C:\Users\PRO\AppData\Local\Android\Sdk\ndk\27.x.x
```

**Verification Command**:
```bash
python scripts/setup_env.py
```

**Expected Output**:
```
✅ OpenXR SDK: C:\OpenXR-SDK (version 1.0.34)
✅ Meta XR SDK: C:\Meta-XR-SDK (version 81.0)
✅ Android NDK: r27
✅ Quest 3 Connected: 1WMHH81234567
⚠️  Quest 3 Developer Mode: Enabled
```

**Documentation Output**: `docs/Environment_Setup.md`

#### D. Building OpenXR SDK for Android

**Prerequisite**: This tutorial requires building OpenXR SDK from source for Android, as no prebuilt Android binaries are available from Khronos.

**Source Repository**: https://github.com/KhronosGroup/OpenXR-SDK-Source

**Build Steps**:

1. **Clone OpenXR-SDK-Source**:
   ```bash
   cd C:\
   git clone https://github.com/KhronosGroup/OpenXR-SDK-Source.git
   cd OpenXR-SDK-Source
   ```

2. **Configure CMake for Android**:
   ```bash
   mkdir build-android
   cd build-android

   cmake .. \
     -G "Ninja" \
     -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK_ROOT%/build/cmake/android.toolchain.cmake \
     -DANDROID_ABI=arm64-v8a \
     -DANDROID_PLATFORM=android-34 \
     -DCMAKE_BUILD_TYPE=Release \
     -DCMAKE_INSTALL_PREFIX=C:/OpenXR-SDK
   ```

3. **Build and Install**:
   ```bash
   cmake --build . --config Release
   cmake --install . --config Release
   ```

4. **Verify Installation**:
   ```bash
   # Check headers
   dir C:\OpenXR-SDK\include\openxr\openxr.h

   # Check Android loader library
   dir C:\OpenXR-SDK\lib\android\arm64-v8a\libopenxr_loader.so

   # Check CMake config
   dir C:\OpenXR-SDK\share\openxr\cmake\OpenXRConfig.cmake
   ```

**Expected Result**:
```
C:\OpenXR-SDK\
├── include\openxr\
│   ├── openxr.h
│   ├── openxr_platform.h
│   └── openxr_reflection.h
├── lib\android\arm64-v8a\
│   └── libopenxr_loader.so
└── share\openxr\cmake\
    ├── OpenXRConfig.cmake
    └── OpenXRConfigVersion.cmake
```

**Troubleshooting**:
- If Ninja not found: Install via `choco install ninja` or use Visual Studio generator
- If NDK not found: Set `ANDROID_NDK_ROOT` environment variable
- If CMake fails: Ensure CMake 3.22+ installed

**Alternative Build Tools**:
- **Gradle/AGP**: Can also build using Android Gradle Plugin (slower but more integrated)
- **Android Studio**: Can import CMakeLists.txt and build from IDE

**Future Platforms** (not needed for this tutorial):
- **Windows**: Use Visual Studio generator, install to same `C:\OpenXR-SDK`
- **macOS**: Use Xcode or Ninja, install to `/usr/local/OpenXR-SDK`
- **Linux**: Use Ninja or Make, install to `/opt/OpenXR-SDK` or `~/.local/OpenXR-SDK`

---

### **Phase 1: Repository Foundation** (Structure)

**Goal**: Create tutorial repository with documentation framework

**Steps**:

1. **Create repository structure**
   ```bash
   mkdir openxr-quest-tutorial
   cd openxr-quest-tutorial
   git init
   ```

2. **Create documentation framework**
   - `CLAUDE.md` - AI Agent master instructions (repo root)
   - `docs/Environment_Setup.md` - SDK installation guide
   - `docs/Build_Deploy_Guide.md` - Generic build/deploy workflow
   - `README.md` - Human-readable project overview

3. **Root build configuration**
   - `settings.gradle` - Multi-project setup
   - `build.gradle` - Shared dependency versions
   - `.gitignore` - Standard Android/CMake ignores

4. **Automation scripts**
   - `scripts/build.py` - Generic build script
   - `scripts/deploy.py` - Generic Quest 3 deployment
   - `scripts/setup_env.py` - Verify SDK installations

**Output**: Empty repository structure ready for samples

---

### **Phase 2: Extract hello_xr Sample** (First Sample)

**Goal**: Migrate hello_xr with system SDK dependencies + preserved analysis docs

**Source**: `OpenXR-SDK-Source/src/tests/hello_xr/`

**Steps**:

1. **Copy source files**
   ```
   samples/hello_xr/
   ├── src/
   │   ├── main.cpp
   │   ├── openxr_program.cpp
   │   ├── openxr_program.h
   │   ├── graphicsplugin_vulkan.cpp
   │   ├── graphicsplugin_opengles.cpp
   │   ├── platformplugin_android.cpp
   │   ├── geometry.h
   │   ├── logger.cpp
   │   └── ... (all source files)
   ├── vulkan_shaders/
   ├── android_resources/
   ├── CMakeLists.txt
   ├── build.gradle
   ├── settings.gradle
   ├── AndroidManifest.xml
   └── README.md
   ```

2. **Update CMakeLists.txt**

   **OLD (original repo)**:
   ```cmake
   # Assumes parent project builds OpenXR loader
   target_link_libraries(hello_xr PRIVATE openxr_loader)
   ```

   **NEW (system SDK)**:
   ```cmake
   # Use system-installed OpenXR SDK
   find_package(OpenXR REQUIRED)
   target_link_libraries(hello_xr PRIVATE OpenXR::openxr_loader)
   ```

3. **Update build.gradle**

   **OLD**:
   ```gradle
   android {
       externalNativeBuild {
           cmake {
               path "CMakeLists.txt"
           }
       }
   }
   ```

   **NEW**:
   ```gradle
   android {
       externalNativeBuild {
           cmake {
               path "CMakeLists.txt"
               arguments "-DCMAKE_PREFIX_PATH=$System.env.OPENXR_SDK_ROOT"
           }
       }
   }
   ```

4. **Migrate documentation**
   - Move `Application_Logic.md` → `docs/samples/hello_xr_analysis.md`
   - Move `Developers_Guidelines.md` → `docs/samples/hello_xr_guidelines.md`
   - Move `Developers_Guidelines_Windows.md` → `docs/samples/hello_xr_guidelines_windows.md`
   - Create `samples/hello_xr/README.md` with quick build instructions

5. **Test independent build**
   ```bash
   cd samples/hello_xr
   ./gradlew assembleDebug
   # Should build without any reference to OpenXR-SDK-Source parent repo
   ```

**Expected Result**:
- `samples/hello_xr/build/outputs/apk/debug/hello_xr-debug.apk`
- Build succeeds using only system OpenXR SDK
- No dependency on OpenXR-SDK-Source repository

**Output**: Standalone hello_xr sample with preserved documentation

---

### **Phase 3: Extract Meta Samples** (Bulk Migration)

**Goal**: Import all 19 Meta samples referencing system Meta XR SDK

**Source**: `ovr_openxr_mobile_sdk_81.0/Samples/XrSamples/`

**Meta Samples List** (19 total):
1. XrBodyFaceEyeSocial
2. XrColocationDiscovery
3. XrColorSpaceFB
4. XrCompositor_NativeActivity
5. XrControllers
6. XrDynamicObjects
7. XrHandDataSource
8. XrHandsAndControllers
9. XrHandsFB
10. XrHandTrackingWideMotionMode
11. XrInput
12. XrMicrogestures
13. XrPassthrough
14. XrPassthroughOcclusion
15. XrSceneModel
16. XrSceneSharing
17. XrSpaceWarp
18. XrSpatialAnchor
19. XrVirtualKeyboard

**Steps**:

1. **Extract shared framework**

   **Decision Point**: Framework location strategy
   - **Option A**: Copy into repo (`samples/meta/_framework/`)
   - **Option B**: Reference from system SDK (`$META_XR_SDK_ROOT/SampleXrFramework/`)

   **Recommendation**: **Option A** (repo copy) for version stability

   ```
   samples/meta/_framework/
   └── (copy from $META_XR_SDK_ROOT/SampleXrFramework/)
       ├── src/
       ├── include/
       └── CMakeLists.txt
   ```

2. **Copy all Meta samples**
   ```bash
   # From OVR SDK
   cp -r "$META_XR_SDK_ROOT/Samples/XrSamples/*" samples/meta/
   ```

   Result:
   ```
   samples/meta/
   ├── _framework/
   ├── XrPassthrough/
   ├── XrHandsFB/
   ├── XrControllers/
   └── ... (all 19 samples)
   ```

3. **Update CMakeLists.txt for each sample**

   **OLD (original)**:
   ```cmake
   # Assumes parent CMake project structure
   add_subdirectory(../../SampleXrFramework SampleXrFramework)
   ```

   **NEW (tutorial repo)**:
   ```cmake
   # Reference system Meta SDK for extensions
   include_directories($ENV{META_XR_SDK_ROOT}/OpenXR/Include)

   # Reference local framework copy
   add_subdirectory(../_framework SampleXrFramework)

   # Link framework
   target_link_libraries(${PROJECT_NAME} PRIVATE SampleXrFramework)
   ```

4. **Update build.gradle for each sample**

   **Add**:
   ```gradle
   android {
       externalNativeBuild {
           cmake {
               arguments "-DMETA_XR_SDK_ROOT=$System.env.META_XR_SDK_ROOT"
           }
       }
   }
   ```

5. **Create master Gradle configuration**

   **Root `settings.gradle`**:
   ```gradle
   rootProject.name = 'OpenXR Quest Tutorial'

   // hello_xr sample
   include ':samples:hello_xr'
   project(':samples:hello_xr').projectDir = file('samples/hello_xr')

   // Meta samples
   include ':samples:meta:XrPassthrough'
   include ':samples:meta:XrHandsFB'
   include ':samples:meta:XrControllers'
   include ':samples:meta:XrCompositor_NativeActivity'
   include ':samples:meta:XrBodyFaceEyeSocial'
   include ':samples:meta:XrColocationDiscovery'
   include ':samples:meta:XrColorSpaceFB'
   include ':samples:meta:XrDynamicObjects'
   include ':samples:meta:XrHandDataSource'
   include ':samples:meta:XrHandsAndControllers'
   include ':samples:meta:XrHandTrackingWideMotionMode'
   include ':samples:meta:XrInput'
   include ':samples:meta:XrMicrogestures'
   include ':samples:meta:XrPassthroughOcclusion'
   include ':samples:meta:XrSceneModel'
   include ':samples:meta:XrSceneSharing'
   include ':samples:meta:XrSpaceWarp'
   include ':samples:meta:XrSpatialAnchor'
   include ':samples:meta:XrVirtualKeyboard'

   // Configure project directories
   include ':samples:meta:_framework'
   project(':samples:meta:_framework').projectDir = file('samples/meta/_framework')
   ```

6. **Document Meta extensions**
   - Create `docs/Meta_Extensions_Guide.md` - Overview of Meta FB_* extensions
   - Create `docs/samples/meta_samples_overview.md` - What each sample demonstrates
   - Add README.md to each Meta sample directory

**Test Build**:
```bash
# Build single Meta sample
cd samples/meta/XrPassthrough
./gradlew assembleDebug

# Build all samples (from root)
./gradlew assembleDebug
```

**Output**: All 19 Meta samples buildable independently

---

### **Phase 4: AI Agent Automation** (Generic Workflows)

**Goal**: Create context-aware scripts that work for ANY sample in the repo

#### A. Generic Build Script (`scripts/build.py`)

**Features**:
- Auto-detect current project (from cwd)
- Build configuration selection (Debug/Release, Vulkan/OpenGLES)
- Parallel builds for all samples

**Usage Examples**:
```bash
# From any sample directory:
cd samples/hello_xr
python ../../scripts/build.py --config Debug --api Vulkan

# From root directory:
python scripts/build.py --sample hello_xr --config Release
python scripts/build.py --sample meta/XrPassthrough --config Debug
python scripts/build.py --all  # Build all samples

# Auto-detect current sample:
cd samples/meta/XrHandsFB
python ../../scripts/build.py  # Auto-detects XrHandsFB
```

**Implementation Outline**:
```python
import os
import subprocess
import argparse
from pathlib import Path

def detect_current_sample():
    """Auto-detect sample from current working directory"""
    cwd = Path.cwd()
    # Logic to determine which sample we're in
    pass

def build_sample(sample_name, config, api):
    """Build specific sample"""
    # Run Gradle build with appropriate flags
    pass

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--sample', help='Sample name (auto-detected if not specified)')
    parser.add_argument('--config', choices=['Debug', 'Release'], default='Debug')
    parser.add_argument('--api', choices=['Vulkan', 'OpenGLES'], default='Vulkan')
    parser.add_argument('--all', action='store_true', help='Build all samples')
    args = parser.parse_args()
    # ... implementation
```

**AI Agent Integration** (in `docs/CLAUDE.md`):
```markdown
## Building a Sample

To build the current project, use:
```bash
python scripts/build.py --config Debug --api Vulkan
```

The script will auto-detect the current sample from your working directory.

For specific samples:
```bash
python scripts/build.py --sample hello_xr --config Release
```
```

#### B. Generic Deploy Script (`scripts/deploy.py`)

**Features**:
- Auto-detect Quest 3 via adb
- Install APK + launch
- Stream logcat output
- Handle permissions automatically

**Usage Examples**:
```bash
# From sample directory (auto-detect):
cd samples/hello_xr
python ../../scripts/deploy.py

# From root directory:
python scripts/deploy.py --sample hello_xr
python scripts/deploy.py --sample meta/XrPassthrough

# With logcat streaming:
python scripts/deploy.py --logcat
```

**Implementation Outline**:
```python
import subprocess
import argparse
from pathlib import Path

def detect_quest3():
    """Detect connected Quest 3 via adb"""
    result = subprocess.run(['adb', 'devices'], capture_output=True, text=True)
    # Parse output to find Quest 3
    pass

def install_apk(apk_path, package_name):
    """Install APK and grant permissions"""
    subprocess.run(['adb', 'install', '-r', apk_path])
    # Grant necessary permissions
    pass

def launch_app(package_name, activity_name):
    """Launch application"""
    subprocess.run(['adb', 'shell', 'am', 'start', f'{package_name}/{activity_name}'])
    pass

def stream_logcat(package_filter):
    """Stream logcat output"""
    subprocess.run(['adb', 'logcat', '-s', package_filter])
    pass

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--sample', help='Sample name (auto-detected if not specified)')
    parser.add_argument('--logcat', action='store_true', help='Stream logcat after launch')
    args = parser.parse_args()
    # ... implementation
```

**AI Agent Integration** (in `docs/CLAUDE.md`):
```markdown
## Deploying to Quest 3

After building, deploy with:
```bash
python scripts/deploy.py
```

This will:
1. Detect connected Quest 3
2. Install APK
3. Grant necessary permissions
4. Launch application
5. Stream logcat output (optional: --logcat flag)
```

#### C. Environment Verification Script (`scripts/setup_env.py`)

**Features**:
- Check OPENXR_SDK_ROOT exists and has correct structure
- Check META_XR_SDK_ROOT exists and has correct structure
- Verify NDK installation
- Check adb connectivity to Quest 3
- Generate environment report

**Usage**:
```bash
python scripts/setup_env.py
```

**Expected Output**:
```
========================================
OpenXR Quest Tutorial - Environment Check
========================================

SDK Installations:
✅ OpenXR SDK: C:\OpenXR-SDK (version 1.0.34)
   - Headers: C:\OpenXR-SDK\include\openxr\openxr.h
   - Android Loader: C:\OpenXR-SDK\lib\android\arm64-v8a\libopenxr_loader.so
   - CMake Config: C:\OpenXR-SDK\share\openxr\cmake\OpenXRConfig.cmake

✅ Meta XR SDK: C:\Meta-XR-SDK (version 81.0)
   - Extensions: C:\Meta-XR-SDK\OpenXR\Include
   - Framework: C:\Meta-XR-SDK\SampleXrFramework
   - Libraries: C:\Meta-XR-SDK\OpenXR\Libs\Android\arm64-v8a

✅ Android NDK: r27 (C:\Users\PRO\AppData\Local\Android\Sdk\ndk\27.0.12077973)

Development Tools:
✅ adb: Version 34.0.5
✅ Gradle: 8.10.0
✅ CMake: 3.22.1

Device Connectivity:
✅ Quest 3 Connected: 1WMHH81234567
✅ Developer Mode: Enabled
✅ USB Debugging: Authorized

========================================
Environment Status: ✅ ALL CHECKS PASSED
========================================
```

**Implementation Outline**:
```python
import os
import subprocess
from pathlib import Path

def check_openxr_sdk():
    """Verify OpenXR SDK installation"""
    sdk_root = os.getenv('OPENXR_SDK_ROOT')
    if not sdk_root:
        return False, "OPENXR_SDK_ROOT not set"
    # Check for headers, libraries, cmake configs
    pass

def check_meta_sdk():
    """Verify Meta XR SDK installation"""
    sdk_root = os.getenv('META_XR_SDK_ROOT')
    if not sdk_root:
        return False, "META_XR_SDK_ROOT not set"
    # Check for extensions, framework, libraries
    pass

def check_ndk():
    """Verify Android NDK installation"""
    ndk_root = os.getenv('ANDROID_NDK_ROOT')
    # Verify NDK version and structure
    pass

def check_quest3():
    """Check Quest 3 connectivity"""
    result = subprocess.run(['adb', 'devices', '-l'], capture_output=True, text=True)
    # Parse for Quest 3 device
    pass

def main():
    print("=" * 50)
    print("OpenXR Quest Tutorial - Environment Check")
    print("=" * 50)
    # Run all checks and report
    pass
```

---

### **Phase 5: Documentation for AI Agents** (Multi-Agent Support)

**Goal**: Create documentation that works for Claude Code + future agents (Codex, Gemini, Grok)

#### `CLAUDE.md` - Structure (Repository Root)

```markdown
# OpenXR Quest Tutorial - Claude Code Guide

> AI Agent instructions for working with this repository

---

## Repository Purpose

This is a **tutorial and learning repository** containing consolidated OpenXR samples:
- `samples/hello_xr/` - Basic OpenXR sample from Khronos
- `samples/meta/` - 19 Meta Quest samples (passthrough, hand tracking, controllers, etc.)

**This is NOT a production development repository.**

Production development happens in separate repositories:
- **InnoVision SDK**: Separate repository
- **Customer applications**: Separate repositories

---

## Environment Requirements

Before working with this repository, verify SDKs are installed system-wide:

```bash
python scripts/setup_env.py
```

**Required System Installations**:
- **OpenXR SDK**: `$OPENXR_SDK_ROOT` → `C:\OpenXR-SDK`
- **Meta XR SDK**: `$META_XR_SDK_ROOT` → `C:\Meta-XR-SDK`
- **Android NDK**: Via Android Studio (NDK r27+)

**Installation Instructions**: See [docs/Environment_Setup.md](docs/Environment_Setup.md)

---

## Working with Samples

### Context-Aware Operations

When the user requests an operation like "build debug" or "deploy to Quest":

1. **Detect current working directory**
   - Identify which sample project we're in
   - Example: `samples/meta/XrPassthrough/`

2. **Apply generic script for that sample**
   - Use auto-detection features of build/deploy scripts

3. **Use appropriate configuration**
   - Debug/Release based on user request
   - Vulkan/OpenGLES based on sample default or user preference

### Building Samples

**Generic build command** (works from any sample directory):

```bash
# From sample directory (auto-detects project):
python ../../scripts/build.py --config Debug --api Vulkan

# From root directory (specify sample):
python scripts/build.py --sample hello_xr --config Release
python scripts/build.py --sample meta/XrPassthrough --config Debug

# Build all samples:
python scripts/build.py --all
```

**Build Configurations**:
- **Config**: `Debug` | `Release`
- **Graphics API**: `Vulkan` | `OpenGLES`

**Default**: `Debug` + `Vulkan`

### Deploying Samples

**Generic deploy command** (auto-detects current sample):

```bash
# From sample directory:
python ../../scripts/deploy.py

# From root directory:
python scripts/deploy.py --sample meta/XrHandsFB

# With logcat streaming:
python scripts/deploy.py --logcat
```

**Deployment Process**:
1. Detect connected Quest 3 via adb
2. Install APK (build output from previous build step)
3. Grant necessary permissions (camera, storage, etc.)
4. Launch application
5. Optionally stream logcat output

---

## Common User Requests & Responses

### "Build and deploy hello_xr in Debug mode"

**Actions**:
```bash
cd samples/hello_xr
python ../../scripts/build.py --config Debug --api Vulkan
python ../../scripts/deploy.py
```

### "Deploy Debug configuration"

**Context-aware interpretation**:
1. Detect current working directory → identify sample
2. Build in Debug mode
3. Deploy to Quest 3

**Actions**:
```bash
python ../../scripts/build.py --config Debug
python ../../scripts/deploy.py
```

### "Build all Meta samples"

**Actions**:
```bash
python scripts/build.py --all --filter meta/
```

### "Show me how XrPassthrough works"

**Response**:
1. Read `samples/meta/XrPassthrough/README.md` (build instructions)
2. Read `docs/samples/meta_passthrough_analysis.md` (deep dive)
3. Summarize architecture and key OpenXR calls

---

## Documentation Structure

### AI Agent Documentation
- `CLAUDE.md` - This file (Claude Code instructions, repo root)
- `docs/AI_Agent_Guide.md` - Future multi-agent support
- `docs/Environment_Setup.md` - SDK installation guide
- `docs/Build_Deploy_Guide.md` - Detailed build/deploy workflows

### Sample Analysis Documentation
- `docs/samples/hello_xr_analysis.md` - Deep dive into hello_xr architecture
- `docs/samples/hello_xr_guidelines.md` - Development guidelines for hello_xr
- `docs/samples/meta_samples_overview.md` - Overview of all 19 Meta samples
- `docs/samples/meta_passthrough_analysis.md` - XrPassthrough deep dive
- `docs/samples/meta_hands_analysis.md` - XrHandsFB deep dive
- ... (additional per-sample analysis)

### Per-Sample Documentation
- `samples/hello_xr/README.md` - Quick build/run instructions
- `samples/meta/XrPassthrough/README.md` - Quick build/run instructions
- ... (each sample has its own README)

---

## Multi-Project Build System

This repository uses Gradle multi-project configuration.

**Root `settings.gradle`**:
- Includes all samples as subprojects
- Allows building individual samples or all at once

**Building Individual Sample**:
```bash
./gradlew :samples:hello_xr:assembleDebug
./gradlew :samples:meta:XrPassthrough:assembleDebug
```

**Building All Samples**:
```bash
./gradlew assembleDebug
```

---

## Troubleshooting

### Build Failures

**Common Issues**:
1. **Missing SDK environment variables**
   - Run `python scripts/setup_env.py` to diagnose
   - Ensure `OPENXR_SDK_ROOT` and `META_XR_SDK_ROOT` are set

2. **CMake cannot find OpenXR**
   - Check `CMAKE_PREFIX_PATH` includes OpenXR SDK
   - Verify `C:\OpenXR-SDK\share\openxr\cmake\OpenXRConfig.cmake` exists

3. **NDK not found**
   - Check `ANDROID_NDK_ROOT` environment variable
   - Verify NDK installation via Android Studio SDK Manager

### Deployment Failures

**Common Issues**:
1. **Quest 3 not detected**
   - Run `adb devices` to verify connection
   - Enable Developer Mode on Quest 3
   - Authorize USB debugging

2. **Permission denials**
   - Deployment script should auto-grant permissions
   - Manually grant via: `adb shell pm grant <package> <permission>`

3. **App crashes on launch**
   - Check logcat: `adb logcat -s hello_xr:*`
   - Verify OpenXR runtime on Quest 3 is up to date

---

## Future Agent Support

### Multi-Agent Architecture

This documentation is designed for Claude Code, but can be extended for other AI agents:

- **Claude Code**: `CLAUDE.md` (this file, repo root)
- **GitHub Copilot/Codex**: `docs/AI_Agent_Guide.md` (future)
- **Google Gemini**: `docs/AI_Agent_Guide.md` (future)
- **Grok**: `docs/AI_Agent_Guide.md` (future)

**Extensibility**: Each agent may have specific conventions for tool use, file operations, etc. Agent-specific instructions can be added to `docs/AI_Agent_Guide.md` with sections for each agent.

---

## Production Development Separation

**Important**: This repository is for **learning and reference only**.

**Production development** (InnoVision SDK, customer applications) should:
1. Use separate Git repositories
2. Reference system-level SDKs (same as this tutorial repo)
3. Use similar build/deploy scripts adapted for production workflows
4. Maintain separate documentation

**Benefits of Separation**:
- Tutorial repo stays clean and focused on learning
- Production code has different quality/testing requirements
- Easier to share tutorial repo publicly (if desired)
- Clear separation of concerns

---

## Version Information

- **Document Version**: 1.0 (DRAFT)
- **Last Updated**: 2025-12-03
- **Target Platform**: Meta Quest 3 (Horizon OS v81, Android API 34)
- **OpenXR Version**: 1.0.34+
- **Meta XR SDK Version**: 81.0

---

**End of Claude Code Guide**
```

#### `docs/AI_Agent_Guide.md` - Placeholder for Future Agents

```markdown
# AI Agent Guide - Multi-Agent Support

> **Future Extension Point**
> This document will contain agent-specific instructions for non-Claude agents

---

## Supported Agents

### Claude Code
Primary documentation: [../CLAUDE.md](../CLAUDE.md)

### GitHub Copilot / OpenAI Codex
**Status**: Not yet implemented

**Planned features**:
- Inline code completion for OpenXR API calls
- Context-aware suggestions for sample modifications
- TBD: Specific instructions for Copilot integration

### Google Gemini
**Status**: Not yet implemented

**Planned features**:
- Multi-modal documentation (code + diagrams)
- TBD: Gemini-specific instructions

### Grok (xAI)
**Status**: Not yet implemented

**Planned features**:
- TBD: Grok-specific instructions

---

## Contributing Agent Support

To add support for a new AI agent:

1. Add a section to this document with agent-specific instructions
2. Document tool/API conventions for that agent
3. Provide examples of common operations in that agent's syntax
4. Test with representative samples

---

**Document Version**: 1.0 (DRAFT)
**Last Updated**: 2025-12-03
```

---

## Workflow Summary Diagram

```
┌─────────────────────────────────────────────────────────────┐
│ Phase 0: Computer-Level SDK Setup                          │
│   Install: OpenXR SDK → C:\OpenXR-SDK                      │
│   Install: Meta XR SDK → C:\Meta-XR-SDK                    │
│   Install: Android NDK (via Android Studio)                │
│   Set: Environment variables (OPENXR_SDK_ROOT, etc.)       │
│   Verify: python scripts/setup_env.py                      │
└─────────────────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────┐
│ Phase 1: Repository Foundation                             │
│   Create: Repository structure                             │
│   Create: docs/CLAUDE.md, docs/Environment_Setup.md        │
│   Create: scripts/build.py, scripts/deploy.py              │
│   Create: Root build.gradle, settings.gradle               │
└─────────────────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────┐
│ Phase 2: Extract hello_xr Sample                           │
│   Source: OpenXR-SDK-Source/src/tests/hello_xr/            │
│   Copy: All source files → samples/hello_xr/               │
│   Update: CMakeLists.txt (use system OpenXR SDK)           │
│   Update: build.gradle (reference system SDK)              │
│   Migrate: Documentation → docs/samples/                   │
│   Test: Independent build (./gradlew assembleDebug)        │
└─────────────────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────┐
│ Phase 3: Extract Meta Samples (19 samples)                 │
│   Source: ovr_openxr_mobile_sdk_81.0/Samples/XrSamples/    │
│   Copy: SampleXrFramework → samples/meta/_framework/       │
│   Copy: All 19 samples → samples/meta/                     │
│   Update: Each CMakeLists.txt (use system Meta SDK)        │
│   Update: Root settings.gradle (include all samples)       │
│   Document: Meta extensions guide                          │
│   Test: Build individual samples                           │
└─────────────────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────┐
│ Phase 4: AI Agent Automation                               │
│   Create: scripts/build.py (context-aware building)        │
│   Create: scripts/deploy.py (context-aware deployment)     │
│   Create: scripts/setup_env.py (environment verification)  │
│   Test: Auto-detection from various sample directories     │
└─────────────────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────────────────┐
│ Phase 5: AI Agent Documentation                            │
│   Write: docs/CLAUDE.md (complete AI agent guide)          │
│   Write: docs/AI_Agent_Guide.md (multi-agent placeholder)  │
│   Write: Per-sample analysis documentation                 │
│   Test: AI agent can successfully build/deploy samples     │
└─────────────────────────────────────────────────────────────┘
```

---

## Key Architecture Decisions

### Decision 1: SDK Installation Location

**Decision**: Install OpenXR SDK and Meta XR SDK at computer level (not in repository)

**Rationale**:
- Tutorial repository stays lean (only source code + docs)
- SDKs are shared across multiple projects (tutorial + production)
- Easier to update SDK versions globally
- Standard practice for system-level libraries

**Implementation**:
- OpenXR SDK → `C:\OpenXR-SDK`
- Meta XR SDK → `C:\Meta-XR-SDK`
- Environment variables: `OPENXR_SDK_ROOT`, `META_XR_SDK_ROOT`

**Trade-offs**:
- ✅ Pro: Clean repository, faster clones, shared SDK
- ⚠️ Con: Requires initial setup (documented in `docs/Environment_Setup.md`)

---

### Decision 2: Documentation Location

**Decision**: `CLAUDE.md` at repository root, detailed docs in centralized `docs/` directory

**Rationale**:
- `CLAUDE.md` at repo root is standard AI agent convention
- Root-level CLAUDE.md provides quick entry point for AI agents
- Detailed documentation in `docs/` keeps root clean
- Cleaner sample directories (focus on code)
- Easier to maintain cross-references
- Supports multi-agent architecture (agent-specific docs)

**Implementation**:
- AI agent master instructions → `CLAUDE.md` (repo root)
- Multi-agent support → `docs/AI_Agent_Guide.md`
- Sample analysis → `docs/samples/`
- Setup guides → `docs/Environment_Setup.md`, `docs/Build_Deploy_Guide.md`
- Per-sample quick reference → `samples/*/README.md`

**Trade-offs**:
- ✅ Pro: Standard convention, centralized, organized, multi-agent ready
- ✅ Pro: Root-level CLAUDE.md is immediately discoverable
- ⚠️ Con: Documentation separated from code (mitigated by README in each sample)

---

### Decision 3: Build System Architecture

**Decision**: Hybrid CMake (native) + Gradle (Android) with context-aware Python scripts

**Rationale**:
- CMake: Industry standard for OpenXR/native C++ compilation
- Gradle: Required for Android APK packaging and Quest 3 deployment
- Python scripts: AI agent can call generic scripts that auto-detect context

**Implementation**:
```
CMake (Native Layer)
  └─> Compiles: C++ source, links OpenXR/Meta SDKs
      └─> Gradle (Android Layer)
          └─> Packages: APK with native libs + Java/Kotlin code
              └─> Python Scripts (Automation Layer)
                  └─> Auto-detects: Current sample, builds, deploys
```

**AI Agent Workflow**:
1. User: "Deploy Debug to Quest"
2. Agent: `python scripts/build.py --config Debug` (detects current sample)
3. Agent: `python scripts/deploy.py` (installs + launches)

**Trade-offs**:
- ✅ Pro: Context-aware, works from any directory, AI-friendly
- ⚠️ Con: Additional layer of abstraction (mitigated by clear documentation)

---

### Decision 4: Sample Framework Strategy (Meta Samples)

**Decision**: Copy `SampleXrFramework` into repository at `samples/meta/_framework/`

**Rationale**:
- Version stability (framework won't change unexpectedly)
- Repository is self-contained for sample code
- Can document/modify framework if needed for learning purposes

**Alternative Considered**: Reference framework from `$META_XR_SDK_ROOT/SampleXrFramework/`
- Rejected: Updates to system SDK could break samples

**Implementation**:
```cmake
# In each Meta sample's CMakeLists.txt:
add_subdirectory(../_framework SampleXrFramework)
target_link_libraries(${PROJECT_NAME} PRIVATE SampleXrFramework)
```

**Trade-offs**:
- ✅ Pro: Stable, self-contained, can be modified for learning
- ⚠️ Con: Duplicates framework code (acceptable for tutorial repo)

---

### Decision 5: Production Code Separation

**Decision**: Tutorial repository is strictly for learning; production development in separate repos

**Rationale**:
- Clear separation of concerns
- Tutorial repo can be shared publicly (open-source samples)
- Production code has different requirements (testing, CI/CD, proprietary features)
- Prevents mixing learning code with customer code

**Implementation**:
- **This Tutorial Repo**: Open-source samples, documentation, learning materials
- **InnoVision SDK Repo**: Production SDK development
- **Customer App Repos**: Production applications

**Cross-Reference**:
- Production repos reference same system SDKs (`OPENXR_SDK_ROOT`, `META_XR_SDK_ROOT`)
- Can reuse build/deploy script patterns from tutorial repo

**Trade-offs**:
- ✅ Pro: Clean separation, shareable, different lifecycles
- ⚠️ Con: None significant (this is best practice)

---

## Meta SDK Plugin Strategy (Clarification Needed)

### Current Understanding

**Available Meta SDK Packages**:

1. **Meta XR Core SDK** (Native)
   - URL: https://developers.meta.com/horizon/downloads/package/meta-xr-core-sdk
   - Contents: Core OpenXR extensions (C/C++ headers and libraries)
   - Extensions: FB_passthrough, FB_hand_tracking, FB_body_tracking, etc.
   - Size: ~50 MB
   - Use Case: Native NDK development (C/C++)

2. **Meta XR SDK All-in-One UPM** (Unity)
   - URL: https://developers.meta.com/horizon/downloads/package/meta-xr-sdk-all-in-one-upm
   - Contents: Unity Package Manager format (includes Core SDK + Unity assets)
   - Size: ~500 MB
   - Use Case: Unity development only

3. **OVR OpenXR Mobile SDK** (Full Native SDK with Samples)
   - URL: https://developers.meta.com/horizon/downloads/package/oculus-openxr-mobile-sdk
   - Contents: Core SDK + SampleXrFramework + 19 sample projects
   - Size: ~200 MB
   - Use Case: Native development with learning samples (what you currently have)

### Questions for User

1. **SDK Choice for Computer-Level Install**:
   - Recommend: **OVR OpenXR Mobile SDK 81.0** (what you already have)
   - Reason: Includes everything needed for native development + samples
   - Alternative: **Meta XR Core SDK** (smaller, but missing sample framework)
   - **Your preference?**

2. **Unity Support**:
   - Do you need Unity samples in this tutorial repository?
   - If YES → Install All-in-One UPM and add `samples/unity/` section
   - If NO → Stick with native-only samples
   - **Your preference?**

3. **Extension Strategy**:
   - Core extensions (FB_passthrough, FB_hand_tracking) are included in OVR SDK
   - Experimental/preview extensions - separate download?
   - How to handle future extensions?
   - **Your guidance?**

4. **Plugin Definition**:
   - By "plugins", do you mean:
     - A) SDK packages (Meta Core vs All-in-One)?
     - B) Custom native plugins (Vulkan renderers, InnoVision SDK)?
     - C) Third-party OpenXR extensions?
   - **Please clarify**

### Recommendations (Pending Your Feedback)

**Recommended Computer-Level SDK Setup**:
```
C:\OpenXR-SDK\              # Khronos OpenXR SDK (version 1.0.34+)
C:\Meta-XR-SDK\             # OVR OpenXR Mobile SDK 81.0
C:\Android\Sdk\ndk\27.x\    # Android NDK r27 (via Android Studio)
```

**If Unity Support Needed** (future):
```
C:\Meta-XR-Unity\           # Meta XR SDK All-in-One UPM
```

---

## Open Questions for User

### 1. Repository Name

**Decision**: ✅ `openxr-quest-tutorial`

**Rationale**: While some samples may be cross-platform, the focus is currently on the Quest flavor of NDK+OpenXR development. The name reflects this primary target platform.

---

### 2. OpenXR SDK Build Strategy

**Decision**: ✅ **Build from source (OpenXR-SDK-Source)** - Required for Android

**Rationale**:
- Prebuilt binaries for Android (Quest) do not exist
- Windows prebuilts exist but are not needed for this Quest-focused tutorial
- Building from source provides:
  - Latest Android/Quest support
  - Ability to customize if needed
  - Understanding of SDK build process

**Source**: https://github.com/KhronosGroup/OpenXR-SDK-Source

**Note**: Windows/Mac development documentation is retained for reference, but requires building OpenXR SDK for those platforms if needed in the future.

---

### 3. Gradle/AGP Versions

**Match InnoVision Setup**:
- Gradle: 8.10.0
- AGP: 8.13.0 or 8.10.0
- Android API: 34
- NDK: r27

**Confirmed?**

---

### 4. Windows Development Support

**Decision**: ✅ **Android/Quest 3 primary focus, Windows/Mac docs for future reference**

**Rationale**:
- Tutorial focuses on Quest flavor of NDK+OpenXR
- Windows/Mac development documentation retained for completeness
- If Windows/Mac support needed in future:
  - Build OpenXR SDK from source for those platforms
  - Add platform-specific build configurations
  - Desktop VR testing (Valve Index, HTC Vive, etc.)

**Current Implementation**: Android/Quest 3 only, with cross-platform documentation notes

---

### 5. Git Repository Location

**Decision**: ✅ **New standalone repository**

**Location**: `C:\Users\PRO\Projects\openxr-quest-tutorial\`

**Rationale**:
- Independent from InnoVision projects
- Clean separation of tutorial vs production code
- Can be shared publicly in future if desired
- Clear focus on learning and reference

**Status**: Repository created with initial structure

---

### 6. Immediate Next Steps

**What should we start with?**

**Option A**: Phase 0 (SDK Installation Documentation)
- Document computer-level SDK setup
- Create `docs/Environment_Setup.md`
- Create `scripts/setup_env.py` verification script

**Option B**: Phase 1 (Repository Structure)
- Create repository skeleton
- Set up build configuration
- Create initial documentation framework

**Option C**: Combination
- Do both Phase 0 and Phase 1 in parallel

**Your preference?**

---

## Summary of User Feedback Needed

Please provide feedback on:

1. ✅ **Repository name** → **CONFIRMED: `openxr-quest-tutorial`** (Quest-focused NDK+OpenXR)
2. ✅ **OpenXR SDK build** → **CONFIRMED: Build from source** (no Android prebuilts exist)
3. ✅ **Windows support** → **CONFIRMED: Android/Quest 3 only** (Windows/Mac docs retained for reference)
4. ✅ **Git location** → **CONFIRMED: Standalone repo** at `C:\Users\PRO\Projects\openxr-quest-tutorial\`
5. ✅ **Meta SDK strategy** (confirm OVR OpenXR Mobile SDK 81.0?)
6. ✅ **Unity support** (needed or native-only?)
7. ✅ **Plugin definition** (SDK packages, custom plugins, or extensions?)
8. ✅ **Gradle/AGP versions** (match InnoVision: Gradle 8.10, AGP 8.13?)
9. ✅ **Immediate priority** (Phase 0, Phase 1, or both?)

---

## Estimated Effort

| Phase | Description | Estimated Time |
|-------|-------------|----------------|
| **Phase 0** | SDK Installation Docs | 2-3 hours |
| **Phase 1** | Repository Foundation | 2-3 hours |
| **Phase 2** | Extract hello_xr | 1-2 hours |
| **Phase 3** | Extract Meta Samples (19) | 4-6 hours |
| **Phase 4** | AI Agent Automation Scripts | 2-3 hours |
| **Phase 5** | AI Agent Documentation | 2-3 hours |
| **Total** | | **13-20 hours** |

**Note**: Times assume SDK installation decisions are finalized.

---

**Document Version**: 1.0
**Created**: 2025-12-03
**Last Updated**: 2025-12-03
**Repository**: `C:\Users\PRO\Projects\openxr-quest-tutorial\`
**Status**: Active - Repository created, beginning Phase 0/1 implementation

---

**End of OpenXR Quest Tutorial - Project Architecture & Implementation Plan**
