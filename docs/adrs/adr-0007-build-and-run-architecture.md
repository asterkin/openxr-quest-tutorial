# ADR-0007: Build and Run Architecture

**Status**: Proposed

## Context

OpenXR Quest samples require a build and deployment workflow that supports:
- Native C++ compilation for Android (CMake + NDK)
- Android APK packaging and signing (Gradle)
- Device deployment and testing (ADB)
- Developer convenience and automation

The build system must balance:
- **Standard tooling**: Follow Android/CMake conventions
- **Developer productivity**: Simple commands for common tasks
- **Automation readiness**: Scriptable for CI/CD and AI agents
- **Tutorial clarity**: Learners understand the build flow

### Current Implementation (hello_xr)

The `hello_xr` sample uses:
1. **Gradle wrapper** (`gradlew.bat`) for build and APK generation
2. **CMake** (invoked by Gradle) for native C++ compilation
3. **Batch scripts** (`test_run.bat`, `adb_cleanup.bat`) for device operations
4. **Manual ADB commands** for deployment and logging

This approach works but has limitations:
- Batch scripts are Windows-only
- No unified workflow automation
- Manual coordination between build/deploy/test steps

### Future Needs

As more samples are added:
- Need cross-platform automation (Windows, macOS, Linux)
- Context-aware scripts (detect current sample directory)
- Unified workflows for AI agents
- CI/CD pipeline integration

## Decision

Adopt a **layered build and run architecture** with three levels:

### Layer 1: Build (Gradle + CMake)

**Primary Interface**: Gradle wrapper (`gradlew.bat` / `gradlew`)

**Responsibilities**:
- APK packaging and signing
- Dependency resolution (Maven, CMake)
- Invokes CMake for native builds
- Invokes NDK toolchain

**Commands**:
```bash
# Build Vulkan debug APK
gradlew assembleVulkanDebug

# Build OpenGL ES debug APK
gradlew assembleOpenGLESDebug

# Clean build
gradlew clean
```

**CMake Role**:
- Invoked automatically by Gradle's `externalNativeBuild`
- Compiles C++ sources
- Links OpenXR loader and platform libraries
- Managed via `CMakeLists.txt` in each sample

### Layer 2: Device Operations (ADB + Batch Scripts)

**Primary Tool**: Android Debug Bridge (ADB)

**Current Approach** (hello_xr sample):
- **Batch scripts** for common workflows:
  - `test_run.bat` - Build, install, run, stream logs
  - `adb_cleanup.bat` - Uninstall, clear cache, kill processes
- **Manual ADB** for ad-hoc operations

**Responsibilities**:
- APK installation/uninstallation
- Application launch/stop
- Logcat streaming
- Device cleanup (zombie processes, cached data)

**Commands**:
```bash
# Install APK
adb install -r path/to/app.apk

# Launch application
adb shell am start -n <package>/<activity>

# Stream logs
adb logcat -c && adb logcat -s OpenXR:* *:E

# Cleanup
adb shell pm clear <package>
```

### Layer 3: Workflow Automation (Python - Future)

**Status**: Not yet implemented

**Planned Responsibilities**:
- Context-aware build/deploy/test workflows
- Cross-platform support (Windows, macOS, Linux)
- AI agent integration
- Sample auto-detection

**Planned Commands** (example):
```bash
# Auto-detect sample and build
python scripts/build.py --config Debug

# Deploy to device and stream logs
python scripts/deploy.py --run --logs

# Full workflow
python scripts/test.py
```

**Rationale for Future Implementation**:
- Current batch scripts sufficient for Phase 1 (hello_xr)
- Python layer deferred until patterns emerge from more samples
- Avoid premature abstraction

## Consequences

### Advantages

**Layer 1 (Gradle + CMake)**:
- ✅ Industry standard Android/NDK build system
- ✅ IDE integration (Android Studio, VS Code)
- ✅ Dependency management (Maven, FetchContent)
- ✅ Reproducible builds

**Layer 2 (ADB + Batch Scripts)**:
- ✅ Simple, explicit commands
- ✅ Easy to understand for tutorials
- ✅ Immediate developer productivity
- ✅ No additional dependencies (ADB already required)

**Layer 3 (Python - Future)**:
- ✅ Cross-platform automation
- ✅ AI agent integration
- ✅ CI/CD readiness
- ✅ Context-aware workflows

### Disadvantages

**Current Limitations**:
- ⚠️ Batch scripts are Windows-only (hello_xr sample)
- ⚠️ Manual coordination between build/deploy steps
- ⚠️ No unified automation yet

**Trade-offs**:
- ❌ Layer 3 not yet implemented (deferred complexity)
- ❌ Batch scripts require manual porting for macOS/Linux users

### Migration Path

As more samples are added:
1. **Short term**: Continue using batch scripts, add shell script equivalents for macOS/Linux
2. **Medium term**: Implement Python automation layer when patterns stabilize
3. **Long term**: Integrate with CI/CD, AI agent workflows

### Adjustments Expected

This architecture is **deliberately lightweight** for Phase 1. Expected changes:
- Add Python automation layer (Layer 3) when multi-sample patterns emerge
- Refine batch/shell scripts based on developer feedback
- Integrate with CI/CD pipelines (GitHub Actions, GitLab CI)
- Add AI agent integration hooks

Status will be updated from **Proposed** to **Accepted** after:
- Validation with 2-3 additional samples
- Cross-platform testing
- Developer feedback incorporation

## References

- [Project Plan - Decision 3](../Project_Plan.md#L1144-L1171)
- [hello_xr test_run.bat](../../samples/hello_xr/test_run.bat)
- [hello_xr adb_cleanup.bat](../../samples/hello_xr/adb_cleanup.bat)
- [ADR-0005](adr-0005-openxr-sdk-dependency-management-via-maven-and-fetchcontent.md) - OpenXR SDK dependency management
