# ADR-0007: Build and Run Architecture

**Status**: Accepted
**Date**: 2025-12-15

## Context

OpenXR Quest samples require a build and deployment workflow that supports:
- Native C++ compilation for Android (CMake + NDK)
- Android APK packaging (Gradle)
- Device deployment and testing (ADB)

The build system must balance:
- **Standard tooling**: Follow Android/CMake conventions familiar to developers
- **Tutorial clarity**: Learners can understand and modify the build configuration
- **Self-contained samples**: Each sample buildable independently without complex setup
- **Shared code reuse**: Common utilities shared across related samples without duplication

## Decision

### Decision 1: Gradle as Primary Build Interface

**Choice**: Use Gradle wrapper (`gradlew`) as the single entry point for all build operations.

**Rationale**:
- Industry standard for Android development
- IDE integration (Android Studio, VS Code with Gradle extension)
- Handles APK packaging, signing, and dependency resolution
- Invokes CMake automatically via `externalNativeBuild`

**Consequence**: Developers use familiar commands (`gradlew installDebug`) regardless of native code complexity.

### Decision 2: CMake for Native Compilation

**Choice**: CMake 3.22.1+ invoked by Gradle's `externalNativeBuild` block.

**Rationale**:
- NDK's recommended native build system
- FetchContent for dependency management
- Cross-platform CMake scripts (future desktop builds possible)
- Separates native build logic from Android packaging

**Consequence**: Native build configuration in `CMakeLists.txt`, Android configuration in `build.gradle`.

### Decision 3: Hybrid OpenXR SDK Dependency Strategy

**Choice**: Use **both** Maven AAR (loader) and CMake FetchContent (headers/utilities).

```
┌─────────────────────────────────────────────────────────────┐
│                      OpenXR SDK Strategy                     │
├─────────────────────────────────────────────────────────────┤
│  Maven AAR (Prefab)              CMake FetchContent          │
│  ─────────────────               ──────────────────          │
│  • openxr_loader.so              • OpenXR headers            │
│  • Pre-built, tested             • OpenXR utilities          │
│  • Version: 1.1.54               • Version: release-1.1.54   │
│                                                              │
│  build.gradle:                   CMakeLists.txt:             │
│  implementation                  FetchContent_Declare(       │
│    'org.khronos.openxr:            OpenXR                    │
│     openxr_loader_for_android'     URL ...release-1.1.54...) │
└─────────────────────────────────────────────────────────────┘
```

**Rationale**:
- **Loader from Maven**: Pre-built, tested binary; simpler than building from source
- **Headers via FetchContent**: Access to full SDK utilities (`OpenXRDebugUtils`, `xr_linear_algebra.h`)
- **Version alignment**: Both pinned to 1.1.54 for compatibility

**Alternatives Considered**:
- *Maven only*: Lacks access to SDK utility headers and common code
- *FetchContent only*: Slower builds, loader compilation issues with NDK 27+
- *Vendor SDK (Meta OpenXR SDK)*: Adds proprietary dependency, less portable

**Consequence**: Slightly larger configuration but provides best of both approaches.

### Decision 4: Shared Configuration Pattern (openxr-tutorial)

**Choice**: Use Gradle `apply from` and CMake `include()` for shared configuration.

```
samples/openxr-tutorial/
├── Common/
│   ├── app/build.gradle      ← Shared Gradle config (applied by chapters)
│   ├── CMakeLists.txt        ← Shared CMake config (included by chapters)
│   ├── *.cpp / *.h           ← Shared C++ code
├── Chapter1/
│   ├── app/build.gradle      ← apply from: "../Common/app/build.gradle"
│   ├── CMakeLists.txt        ← include("../Common/CMakeLists.txt")
│   ├── main.cpp              ← Chapter-specific code
│   └── settings.gradle       ← Standalone project settings
├── Chapter2/ ...
```

**Rationale**:
- Each chapter remains a standalone Gradle project (no multi-module complexity)
- Shared configuration reduces duplication and ensures consistency
- Chapter-specific code clearly separated
- Easy to understand for tutorial readers

**Alternatives Considered**:
- *Multi-module Gradle project*: More complex, harder to run individual chapters
- *Copy-paste configuration*: Maintenance burden, drift between chapters
- *Git submodules*: Overkill for single-repo tutorials

**Consequence**: Chapters are independently buildable but share common patterns.

### Decision 5: Standalone Samples (hello_world, hello_xr)

**Choice**: Non-tutorial samples are fully self-contained with no shared dependencies.

**Rationale**:
- Minimal setup for quick verification
- No external file dependencies
- Clear reference implementations

**Consequence**: Some duplication between samples, but maximum clarity and portability.

### Decision 6: NativeActivity Integration

**Choice**: All samples use Android's `NativeActivity` with the `-u ANativeActivity_onCreate` linker flag.

**Rationale**:
- No Java/Kotlin code required for pure C++ samples
- Simpler entry point for OpenXR applications
- Standard pattern for native VR applications

**Technical Requirement**:
```cmake
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-u ANativeActivity_onCreate"
)
```

**Consequence**: Linker flag must be explicitly set; without it, runtime crashes with `UnsatisfiedLinkError`.

### Decision 7: Toolchain Versions

**Choice**: Pin specific toolchain versions for reproducibility.

| Component | Version | Rationale |
|-----------|---------|-----------|
| NDK | r27.2.12479018 | Quest 3 / Horizon OS v81 support |
| CMake | 3.22.1+ | NDK r27 bundled version |
| Gradle | 8.x | Android Studio Ladybug compatibility |
| AGP | 8.13.0 | Gradle 8 requirement |
| API Level | 29+ (minSdk) | OpenXR Android runtime requirement |
| API Level | 34 (targetSdk) | Current Android 14 target |

**Consequence**: Build environment must meet minimum versions; documented in Environment_Setup.md.

## Consequences

### Advantages

- **Familiar tooling**: Standard Gradle/CMake workflow
- **IDE support**: Works with Android Studio, VS Code
- **Reproducible builds**: Pinned versions and FetchContent
- **Tutorial-friendly**: Clear separation of concerns
- **Flexible**: Shared config for related samples, standalone for independent ones

### Trade-offs

- **Two dependency mechanisms**: Maven + FetchContent adds configuration complexity
- **FetchContent download**: First build downloads ~10MB OpenXR SDK
- **Version synchronization**: Maven AAR and FetchContent versions must match

### Future Considerations

- **CI/CD Integration**: Gradle commands are scriptable for GitHub Actions
- **Cross-platform**: CMake configuration could support desktop builds
- **Automation**: Python scripts could wrap Gradle/ADB for advanced workflows

## References

- [Build_Guidelines.md](../../samples/Build_Guidelines.md) - Detailed build and ADB commands
- [ADR-0005](adr-0005-openxr-sdk-dependency-management-via-maven-and-fetchcontent.md) - OpenXR SDK dependency management rationale
- [Environment_Setup.md](../Environment_Setup.md) - Development environment requirements
