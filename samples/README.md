# OpenXR Quest Tutorial Samples

> Progressive tutorial samples for learning OpenXR development on Meta Quest 3

---

## Overview

This directory contains self-contained OpenXR tutorial samples, organized by complexity from minimal setup to advanced features.

**Target Platform**: Meta Quest 3 (Horizon OS v81, Android API 34)

---

## Available Samples

### 1. hello_world - Minimal OpenXR Application

**Path**: [hello_world/](hello_world/)

**Complexity**: ⭐ Beginner

**Description**:
Minimal OpenXR application demonstrating basic Quest 3 setup with OpenXR session initialization and rendering loop.

**What You'll Learn**:
- OpenXR instance and session creation
- Graphics API (Vulkan) binding
- Basic rendering loop structure
- Meta Quest 3 device setup

**Prerequisites**: Environment setup complete (see [docs/Environment_Setup.md](../docs/Environment_Setup.md))

**Build Time**: ~2 minutes (first build)

---

### 2. hello_xr - Khronos OpenXR Conformance Sample

**Path**: [hello_xr/](hello_xr/)

**Complexity**: ⭐⭐ Intermediate

**Description**:
Official Khronos OpenXR conformance sample adapted for Quest 3, demonstrating standard OpenXR API usage patterns.

**What You'll Learn**:
- OpenXR swapchain management
- View/projection configuration
- Input handling (controllers)
- Reference space management
- Multi-platform OpenXR patterns

**Prerequisites**: Understanding of hello_world sample

**Build Time**: ~3 minutes (first build)

---

### 3. meta - Meta Quest Samples Collection

**Path**: [meta/](meta/)

**Complexity**: ⭐⭐⭐ Advanced

**Description**:
Collection of Meta Quest-specific samples from OVR SDK, demonstrating Quest platform features and optimizations.

**Sample Categories**:
- Rendering techniques
- Passthrough MR
- Hand tracking
- Spatial anchors
- Performance optimization
- Platform-specific features

**Prerequisites**: Understanding of hello_xr sample

**Build Time**: Varies by sample (~2-5 minutes each)

---

## Sample Structure

Each sample follows this structure:

```
sample_name/
├── README.md                 # Sample-specific documentation
├── app/                      # Android application module
│   ├── src/main/
│   │   ├── AndroidManifest.xml
│   │   ├── cpp/              # Native OpenXR code
│   │   └── java/             # Android/JNI bridge
│   ├── build.gradle          # App module build config
│   └── CMakeLists.txt        # Native build config
├── build.gradle              # Root build file
└── settings.gradle           # Gradle project settings
```

---

## Building and Running

### Build a Single Sample

```powershell
# Navigate to sample directory
cd samples\hello_world

# Build debug APK
.\gradlew assembleDebug

# Install to connected Quest 3
.\gradlew installDebug
```

### Build All Samples

```powershell
# From repository root
cd C:\Users\PRO\Projects\openxr-quest-tutorial

# Build all samples (future feature)
.\scripts\build_all.py
```

See [docs/Build_Deploy_Guide.md](../docs/Build_Deploy_Guide.md) for detailed build instructions.

---

## Deployment

### Prerequisites

1. Quest 3 with Developer Mode enabled
2. USB debugging authorized
3. Device connected via USB or Wi-Fi ADB

### Quick Deploy

```powershell
# Verify device connection
adb devices

# Deploy sample (from sample directory)
.\gradlew installDebug

# Launch sample
adb shell am start -n com.example.hello_world/.MainActivity
```

See [docs/Build_Deploy_Guide.md](../docs/Build_Deploy_Guide.md) for deployment workflows.

---

## Learning Path

**Recommended progression**:

1. **Start**: [hello_world/](hello_world/) - Understand minimal OpenXR setup
2. **Next**: [hello_xr/](hello_xr/) - Learn standard OpenXR patterns
3. **Advanced**: [meta/](meta/) - Explore Quest-specific features

Each sample builds on concepts from previous samples.

---

## Documentation

- **Environment Setup**: [docs/Environment_Setup.md](../docs/Environment_Setup.md)
- **Build/Deploy Guide**: [docs/Build_Deploy_Guide.md](../docs/Build_Deploy_Guide.md)
- **Project Plan**: [docs/Project_Plan.md](../docs/Project_Plan.md) - Current status and roadmap
- **AI Agent Instructions**: [CLAUDE.md](../CLAUDE.md)
