# Development Environment Setup

## Target Platform Specifications

- **Device**: Meta Quest 3
- **OS**: Horizon OS v81 (Android API 34)
- **Graphics**: Vulkan 1.0.3+
- **XR Runtime**: OpenXR 1.0.34

---

## Overview

This document describes the **workstation prerequisites** for building the OpenXR Quest Tutorial project.

**Platform**: Windows 10/11 (Build 19045 or higher)
**Shell**: PowerShell 5.1+ or PowerShell Core 7+

---

## Prerequisites Summary

| Component | Version | Purpose |
|-----------|---------|---------|
| **Windows** | 10/11 Pro (Build 19045+) | Host OS |
| **Git for Windows** | 2.x+ | Version control, repository cloning |
| **GitHub CLI** | 2.x+ | GitHub authentication (`gh auth login`), PR/issues management |
| **Claude Code CLI** | Latest | AI-powered development assistant |
| **VSCode** | Latest | Primary IDE for this repository |
| **Java/JDK** | 17 (LTS) | Gradle build system |
| **Gradle** | 8.x | Build automation (via wrapper) |
| **Android SDK** | Platform 34 | Android build target |
| **Android NDK** | r27 (27.2.12479018) | Native C/C++ compilation |
| **Python** | 3.14 | Build scripts, code generation, diagnostics |
| **Meta XR SDK** | OVR Mobile SDK 81.0 | Quest-specific OpenXR extensions (optional) |
| **Meta Quest Developer Hub** | 6.1.1 | Device deployment/debugging |
| **Meta Quest Link** | Cable or Air | Testing (optional) |

---

## Important: Run PowerShell as Administrator

**All commands in this guide must be run in PowerShell with Administrator privileges.**

### Recommended: Pin PowerShell Admin to Taskbar (One-time Setup)

1. Click **Start Menu**
2. Type: `PowerShell`
3. **Right-click** "Windows PowerShell" (desktop app)
4. Click **"Run as administrator"**

   ![PowerShell Run as Administrator in Start Menu](images/powershell-admin.png)

5. Click "Yes" on User Account Control prompt
6. Once PowerShell opens, **right-click the PowerShell icon in taskbar**
7. Select **"Pin to taskbar"**

**From now on:** Click the pinned icon in taskbar to always open PowerShell as Administrator.

### Verify You're Running as Administrator

```powershell
# Should return True if running as Admin
([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
```

**Expected output:** `True`

**Also check window title bar:** Should display "Administrator: Windows PowerShell"

### Navigate to User Home Directory

**Important**: PowerShell Admin starts in `C:\WINDOWS\system32`. Navigate to your user directory and stay there for all setup steps.

```powershell
# Navigate to user home directory (C:\Users\<YourName>)
cd ~
```

**Expected output**: Prompt changes to `PS C:\Users\<YourName>`

---

## Installation Steps

### Step 1: Verify Operating System

**Requirement**: Windows 10 Pro (Build 19045) or Windows 11 Pro

```powershell
# Check Windows version
[System.Environment]::OSVersion.Version
```

**Expected output**: `Major: 10, Minor: 0, Build: 19045` or higher

**Note**: Windows 11 reports as version "10.0.x" for backwards compatibility (kernel version), even though it's marketed as "Windows 11". This is normal - check the build number instead:
- **Windows 10**: Build 19045 or lower
- **Windows 11**: Build 22000 or higher

---

### Step 2: Install Git for Windows

**Version**: 2.x or higher
**Download**: https://git-scm.com/download/win

#### Installation

1. Download the 64-bit installer
2. Run installer with these settings:
   - ✅ **Default editor**: Use Visual Studio Code (or your preference)
   - ✅ **PATH environment**: Git from the command line and also from 3rd-party software
   - ✅ **Line endings**: Checkout Windows-style, commit Unix-style
   - ✅ **Terminal emulator**: Use Windows' default console window
   - ✅ **Default branch name**: Override with "main"
   - ✅ All other settings: Default

#### Configuration

```powershell
# Verify Git installation
git --version
```

**Expected**: `git version 2.x.x`

```powershell
# Configure Git identity (REQUIRED for commits)
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"

# Verify configuration
git config --global --list
```

**Expected output** (example):
```
user.name=Asher Sterkin
user.email=asher.sterkin@gmail.com
```

**Note**: The `--global` flag configures Git for your **current Windows user account only** (even when running in PowerShell Admin - admin privileges don't change user identity). If multiple developers share the same computer with different Windows accounts, each user must configure Git separately under their own account. This is the recommended approach for development workstations.

#### Install GitHub CLI and Authenticate

**Required for cloning private repositories and team collaboration.**

1. **Download GitHub CLI**:
   - URL: https://cli.github.com/
   - Download the Windows installer (.msi)

2. **Install GitHub CLI**:
   - Run the installer with default settings
   - If `gh` command is not found after installation, close and reopen PowerShell Admin to refresh the PATH

3. **Verify installation**:

```powershell
gh --version
```

**Expected**: `gh version 2.x.x` or higher

4. **Authenticate with GitHub**:

```powershell
gh auth login
```

Follow the interactive prompts:
- **What account do you want to log into?** → GitHub.com
- **What is your preferred protocol for Git operations?** → HTTPS
- **Authenticate Git with your GitHub credentials?** → Yes
- **How would you like to authenticate?** → Login with a web browser

5. **Copy the one-time code** shown in the terminal, press Enter, and complete authentication in your browser

6. **Verify authentication**:

```powershell
gh auth status
```

**Expected output** (example):
```
github.com
  ✓ Logged in to github.com account <your-username> (keyring)
  - Active account: true
  - Git operations protocol: https
  - Token: gho_************************************
  - Token scopes: 'gist', 'read:org', 'repo', 'workflow'
```

#### Clone Repository

```powershell
# Create Projects directory in your user home (C:\Users\<YourName>\Projects)
New-Item -ItemType Directory -Force -Path Projects
cd Projects

# Clone the repository
git clone https://github.com/YOUR_USERNAME/openxr-quest-tutorial.git

# Navigate to repository
cd openxr-quest-tutorial

# Verify repository structure
ls
```

**Expected**: Should show directories: `docs`, `samples`, `scripts`, `README.md`, `CLAUDE.md`

**Your working directory**: `C:\Users\<YourName>\Projects\openxr-quest-tutorial`

---

### Step 3: Install Claude Code CLI

**Purpose**: AI-powered assistant for development tasks, troubleshooting, and documentation navigation

**Documentation**: https://github.com/anthropics/claude-code

#### Installation

```powershell
# Install Claude Code CLI (downloads and runs installer)
irm https://claude.ai/install.ps1 | iex
```

Follow the installer prompts to complete the installation.

#### Verification

```powershell
# Check Claude Code CLI version
claude --version

# Test Claude Code CLI
claude --help
```

#### Authentication

**Note**: Authentication happens automatically on first use. If you have a Claude web chat account, you can use it to authenticate.

```powershell
# First invocation will prompt for authentication
claude
```

The CLI will guide you through the authentication process:
- Choose to authenticate with your existing Claude web account
- Complete authentication in your browser
- Return to the terminal to continue

#### Why Install Early?

Claude Code CLI can assist during the rest of the setup process:
- Troubleshoot installation issues
- Navigate documentation in `docs/` directory
- Answer questions about build configuration
- Debug environment problems

**Example usage during setup**:
```powershell
# Ask Claude about Android Studio installation
claude "Where should I install Android SDK?"

# Get help with build errors
claude "Why is Gradle not finding NDK r27?"
```

---

### Step 4: Install Java Development Kit (JDK) 17

**Version**: JDK 17 (LTS)
**Download**: https://adoptium.net/temurin/releases/?version=17

**Recommendation**: Eclipse Temurin (OpenJDK) - free, no license restrictions

#### Installation

1. Download **Windows x64 .msi installer** for JDK 17
2. Run installer
3. **Important**: Check "Set JAVA_HOME variable" during installation
4. Install to default location: `C:\Program Files\Eclipse Adoptium\jdk-17.x.x`

#### Verification

```powershell
# Check Java version
java -version
```

**Expected**:
```
openjdk version "17.0.x" 2024-xx-xx
OpenJDK Runtime Environment Temurin-17.0.x+x (build 17.0.x+x)
OpenJDK 64-Bit Server VM Temurin-17.0.x+x (build 17.0.x+x, mixed mode, sharing)
```

```powershell
# Verify JAVA_HOME environment variable
$env:JAVA_HOME
```

**Expected**: `C:\Program Files\Eclipse Adoptium\jdk-17.x.x` (or similar)

**Note**: If `JAVA_HOME` is not set, you need to re-run the JDK installer:
- Right-click the installer → "Run as administrator"
- Select "Change" (modify installation)
- Enable the "Set JAVA_HOME variable" option
- Complete the installation
- Close and reopen PowerShell Admin to refresh environment variables

---

### Step 5: Install Android Studio (SDK/NDK Manager)

**Version**: Narwhal 4 Feature Drop (2025.1.4)
**Build**: AI-251.27812.49.2514.14217341
**Download**: https://developer.android.com/studio

**Note**: Android Studio is installed **only for SDK/NDK management**. While command-line tools (`sdkmanager`) exist, Android Studio provides the easiest GUI for managing SDK components and is recommended for beginners following this tutorial.

#### Installation

1. Download installer from https://developer.android.com/studio
2. Run installer with default settings (install location: `C:\Program Files\Android\Android Studio`)
3. Launch Android Studio after installation
4. **Initial Setup Wizard**:
   - Choose "Standard" installation type
   - Select theme (preference)
   - Allow SDK download (~4 GB, will take several minutes)
   - Click "Finish"

#### SDK Components Installation

After Android Studio opens:
1. Click **"More Actions"** → **"SDK Manager"**
2. Or navigate to: **Tools → SDK Manager**

**SDK Platforms Tab:**
- ✅ **Android 14.0 (UpsideDownCake)** - API Level 34
  - Android SDK Platform 34

**SDK Tools Tab** (click "Show Package Details" checkbox):
- ✅ **Android SDK Build-Tools 34.0.0**
- ✅ **NDK (Side by side) 27.2.12479018** ⚠️ **CRITICAL - Exact version required**
  - Expand "NDK (Side by side)" section
  - Check version **27.2.12479018** specifically
  - Do NOT use "latest" - must be r27
- ✅ **CMake 3.22.1**
- ✅ **Android SDK Command-line Tools (latest)**
- ✅ **Android Emulator** (optional)
- ✅ **Google Play services** (optional)

Click **"Apply"** → Accept licenses → Wait for downloads

#### Configure Android SDK Environment Variables

**Important**: Set `ANDROID_HOME` and add platform-tools and CMake to PATH (required for Gradle builds, Quest 3 deployment, and OpenXR SDK building).

```powershell
# Set Android SDK location
$androidSdk = "$env:LOCALAPPDATA\Android\Sdk"

# Set ANDROID_HOME permanently at system level
[System.Environment]::SetEnvironmentVariable('ANDROID_HOME', $androidSdk, 'Machine')

# Add platform-tools and CMake to PATH permanently
$currentPath = [System.Environment]::GetEnvironmentVariable('Path', 'Machine')
$platformTools = "$androidSdk\platform-tools"
$cmakeBin = "$androidSdk\cmake\3.22.1\bin"
[System.Environment]::SetEnvironmentVariable('Path', "$currentPath;$platformTools;$cmakeBin", 'Machine')

# Close and reopen PowerShell Admin to refresh environment variables
```

After reopening PowerShell Admin:

```powershell
# Navigate back to project
cd ~
cd .\Projects\Localize-InnoVision-Gradle-Modules\

# Verify ANDROID_HOME is set
$env:ANDROID_HOME
```

**Expected**: `C:\Users\<YourName>\AppData\Local\Android\Sdk`

```powershell
# Verify NDK r27 installation
Test-Path "$env:ANDROID_HOME\ndk\27.2.12479018"
```

**Expected**: `True`

```powershell
# Verify adb is available
adb --version
```

**Expected**: `Android Debug Bridge version 1.x.x`

```powershell
# Verify cmake is available
cmake --version
```

**Expected**: `cmake version 3.22.1` or higher

---

### Step 6: Install Meta Quest Developer Hub (MQDH)

**Version**: 6.1.1
**Download**: https://developers.meta.com/horizon/downloads/package/oculus-developer-hub-win

**Purpose**: Device management, APK deployment, profiling, logging

#### Installation

```powershell
# Download and run installer
# Install to: C:\Program Files\Meta Quest Developer Hub
```

1. Run installer with defaults
2. Launch MQDH after installation
3. Create/login to Meta developer account (if needed)
   - **See**: [Meta Developer Accounts Management](Meta_Developer_Accounts_Management.md) for guidance on individual vs. team accounts

#### Connect Quest 3 Device

**Prerequisites**:
1. Enable Developer Mode on Quest 3:
   - Install Meta Quest mobile app on phone
   - Pair Quest 3 with phone
   - Settings → Developer Mode → Enable
2. USB cable (USB-C to USB-A/C, USB 3.0 recommended)

```powershell
# Connect Quest 3 via USB
# IMPORTANT: Must be a USB-C DATA cable (not charge-only)
# Connect to USB 3.0 port directly on motherboard (not front panel or hub)
# Some USB ports may not work - try different ports if device is not detected

# Put on headset and accept "Allow USB Debugging" prompt
# Click "Always allow from this computer"

# Verify device connection
adb devices
```

**Expected**:
```
List of devices attached
1WMHH1234567890    device
```

#### MQDH Features Used

- **Device Manager**: View connected devices, OS version
- **App Management**: Install/uninstall APKs
- **Quest Profiler**: Performance profiling (CPU, GPU, memory)
- **Logcat**: View device logs in real-time
- **File Browser**: Access device storage
- **Guardian Setup**: Configure play space

#### Verification

1. Launch MQDH
2. Connect Quest 3 via USB
3. Device should appear in "Device Manager" with green indicator
4. Click device → Should show OS version, storage, battery

---

### Step 7: Configure Visual Studio Code

**Version**: Latest stable
**Download**: https://code.visualstudio.com/

**Purpose**: Primary IDE for code editing, debugging, and project navigation

#### Installation

```powershell
# Download and run installer
# Install to: C:\Users\[username]\AppData\Local\Programs\Microsoft VS Code

# Or use winget
winget install Microsoft.VisualStudioCode
```

1. Run installer with defaults
2. **Important**: Check "Add to PATH" during installation
3. Restart PowerShell Admin after installation

#### Verify Installation

```powershell
# Verify installation
code --version

# Open project in VSCode
cd C:\Users\<YourName>\Projects\openxr-quest-tutorial
code .
```

#### Recommended Extensions

Install these extensions for OpenXR Quest development:

**Note**: We use command-line Gradle (`gradlew.bat`) instead of VSCode Gradle tasks to avoid build interference. The Java extension provides IntelliSense without triggering builds.

```powershell
# Core Java Development (IntelliSense only)
code --install-extension redhat.java

# Native Code Support (C/C++, CMake)
code --install-extension ms-vscode.cpptools
code --install-extension ms-vscode.cmake-tools

# AI Assistance
code --install-extension anthropic.claude-code

# Markdown & Documentation
code --install-extension bierner.markdown-mermaid
```

**Extensions Summary**:
- `redhat.java` - Language Support for Java (linting, IntelliSense, formatting)
- `ms-vscode.cpptools` - C/C++ (native OpenXR code editing)
- `ms-vscode.cmake-tools` - CMake Tools (CMakeLists.txt syntax, build integration)
- `anthropic.claude-code` - Claude Code for VS Code (AI-powered coding assistant)
- `bierner.markdown-mermaid` - Markdown Preview Mermaid Support (documentation diagrams)

#### Verification

```powershell
# List installed extensions
code --list-extensions --show-versions
```

**Expected**: Java, C/C++, CMake, and Claude Code extensions installed

---

### Step 8: Install Python

**Version**: Python 3.14
**Download**: https://www.python.org/downloads/

**Purpose**: Used for build scripts, code generation, and diagnostic utilities in OpenXR SDK development

#### Why Python is Needed

While the current tutorial samples don't have Python scripts yet, Python is used by:
- **OpenXR SDK build system**: Code generation scripts for headers and loaders
- **Investigation and debugging**: Ad-hoc scripts for log analysis, device queries, etc.
- **Future automation**: Planned build automation and device management scripts

**Note**: On Windows 11, Python is **not** included by default and must be installed separately.

#### Installation

1. Download **Windows installer (64-bit)** from https://www.python.org/downloads/
2. Run installer
3. ⚠️ **CRITICAL**: Check **"Add python.exe to PATH"** at the bottom of the first screen
4. Click **"Install Now"** (recommended for most users)
5. After installation, click **"Disable path length limit"** if prompted (optional but recommended)

**Installation location**: `C:\Users\<YourName>\AppData\Local\Programs\Python\Python314\`

#### Verification

Close and reopen PowerShell Admin to refresh PATH, then:

```powershell
# Check Python version
python --version
```

**Expected**: `Python 3.14.x`

```powershell
# Check pip (Python package manager)
pip --version
```

**Expected**: `pip 24.x.x from C:\Users\...\site-packages\pip (python 3.14)`

#### Common Issues

**Issue**: `python : The term 'python' is not recognized...`

**Solution**:
1. Python was not added to PATH during installation
2. Re-run the installer → Select "Modify" → Check "Add Python to environment variables"
3. OR manually add to PATH:

```powershell
# Get Python installation path
$pythonPath = (Get-Command python -ErrorAction SilentlyContinue).Source | Split-Path

# If found, add to PATH permanently
if ($pythonPath) {
    $currentPath = [System.Environment]::GetEnvironmentVariable('Path', 'Machine')
    [System.Environment]::SetEnvironmentVariable('Path', "$currentPath;$pythonPath", 'Machine')
}

# Close and reopen PowerShell Admin
```

#### Optional: Upgrade pip

```powershell
# Upgrade pip to latest version
python -m pip install --upgrade pip
```

---

### Step 9: OpenXR SDK (Managed Automatically)

**No manual installation required!**

OpenXR SDK is automatically managed via:
- **OpenXR Loader**: Maven Central dependency (`org.khronos.openxr:openxr_loader_for_android:1.1.54`)
- **OpenXR Headers**: CMake FetchContent from GitHub (OpenXR-SDK-Source)

**What this means**:
- ✅ No manual SDK building or installation
- ✅ Dependencies version-controlled in `build.gradle` and `CMakeLists.txt`
- ✅ Reproducible builds across different machines
- ✅ First build will download dependencies automatically (requires internet)

**Sample CMakeLists.txt pattern**:
```cmake
# Fetch OpenXR SDK for headers
include(FetchContent)
FetchContent_Declare(
    OpenXR
    URL https://github.com/KhronosGroup/OpenXR-SDK-Source/archive/refs/tags/release-1.1.54.tar.gz
)
FetchContent_MakeAvailable(OpenXR)

# Link OpenXR loader (provided by Maven)
target_link_libraries(hello_xr PRIVATE openxr_loader)
```

**Sample build.gradle pattern**:
```gradle
dependencies {
    implementation 'org.khronos.openxr:openxr_loader_for_android:1.1.54'
}
```

---

## Next Steps

After completing environment setup, you're ready to start working with OpenXR Quest samples.

Return to [CLAUDE.md](../CLAUDE.md) for next steps and project navigation.

---

**Documentation Version**: 1.1
**Last Updated**: 2025-12-04
**Maintained by**: OpenXR Quest Tutorial Team
