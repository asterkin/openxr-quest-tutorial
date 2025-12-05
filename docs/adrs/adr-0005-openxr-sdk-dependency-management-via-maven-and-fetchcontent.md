# ADR-0005: OpenXR SDK Dependency Management via Maven and FetchContent

**Status**: Accepted

## Context

OpenXR samples require the OpenXR SDK for both compile-time (headers) and runtime (loader library). The upstream OpenXR-SDK-Source repository builds samples as part of a parent CMake project, which manages the OpenXR loader as a build artifact.

For a standalone tutorial repository, we need a dependency management strategy that:
- Works without a parent CMake project
- Is reproducible across different workstations
- Pins to specific OpenXR versions
- Minimizes manual installation steps
- Follows modern Android development practices

### Alternatives Considered

**Option 1: Build OpenXR SDK from Source Locally**
- Build OpenXR-SDK-Source manually
- Install headers and libraries to system paths
- Reference from CMakeLists.txt

Disadvantages:
- Requires manual SDK building for every developer
- Version management complexity
- No integration with Gradle dependency system

**Option 2: Vendor OpenXR SDK in Repository**
- Check in pre-built OpenXR loader `.aar` files
- Include headers in repository

Disadvantages:
- Large binary files in Git
- Manual updates required
- Platform-specific binaries
- Violates repository size best practices

**Option 3: Hybrid Approach - Maven + FetchContent (Chosen)**
- OpenXR loader AAR from Maven Central
- OpenXR headers via CMake FetchContent

Advantages:
- Automatic dependency resolution
- Version pinning in build files
- No manual installation required
- Standard Android/Gradle practices

## Decision

We will use a **hybrid dependency management approach**:

1. **OpenXR Loader (Runtime)**: Maven Central dependency in `build.gradle`
2. **OpenXR Headers (Compile-Time)**: CMake FetchContent from GitHub

### Implementation

**Gradle Configuration** (`build.gradle`):
```gradle
dependencies {
    // OpenXR loader AAR from Maven Central
    implementation 'org.khronos.openxr:openxr_loader_for_android:1.1.54'
}
```

**CMake Configuration** (`CMakeLists.txt`):
```cmake
include(FetchContent)
FetchContent_Declare(
    OpenXR
    URL https://github.com/KhronosGroup/OpenXR-SDK-Source/archive/refs/tags/release-1.1.54.tar.gz
)
FetchContent_MakeAvailable(OpenXR)

target_link_libraries(hello_xr PRIVATE openxr_loader)
```

### How It Works

1. **First Build**:
   - Gradle downloads OpenXR loader AAR from Maven Central
   - CMake FetchContent downloads OpenXR SDK headers from GitHub
   - CMake finds `openxr_loader` target provided by Gradle's AAR

2. **Subsequent Builds**:
   - Gradle uses cached AAR from `~/.gradle/caches/`
   - CMake uses cached headers from build directory

3. **Version Updates**:
   - Update version number in both `build.gradle` and `CMakeLists.txt`
   - Clean build to fetch new version

## Consequences

### Advantages

**For Developers**:
- Zero manual OpenXR SDK installation
- First build "just works" (requires internet)
- Consistent versions across team

**For Build System**:
- Declarative dependency management
- Version pinning in build files
- Gradle and CMake use standard mechanisms

**For Maintenance**:
- Single source of truth (Maven Central)
- Clear version upgrade path
- No binary files in Git

### Disadvantages

- Requires internet connection for first build
- Downloads ~10MB from GitHub on first CMake configure
- Slightly slower first build (builds loader from source via CMake)

### Trade-offs

**Maven AAR vs Building from Source**:
- ✅ Faster builds (pre-built native libraries)
- ✅ Smaller download (~2MB AAR vs ~10MB source)
- ❌ Less control over loader build configuration

**FetchContent vs System Headers**:
- ✅ Reproducible builds (pinned version)
- ✅ No system-level installation
- ❌ Re-downloads per project (not shared across samples)

### Version Management

**Current OpenXR Version**: 1.1.54

**Update Procedure**:
1. Check [OpenXR releases](https://github.com/KhronosGroup/OpenXR-SDK-Source/releases)
2. Verify Maven Central has matching loader version
3. Update both `build.gradle` and `CMakeLists.txt`
4. Test build and runtime compatibility

## References

- [OpenXR Loader on Maven Central](https://central.sonatype.com/artifact/org.khronos.openxr/openxr_loader_for_android)
- [OpenXR-SDK-Source Releases](https://github.com/KhronosGroup/OpenXR-SDK-Source/releases)
- [CMake FetchContent Documentation](https://cmake.org/cmake/help/latest/module/FetchContent.html)
- [hello_xr Integration Notes](../../samples/hello_xr/INTEGRATION_NOTES.md#L126-L156)
- [ADR-0002](adr-0002-workstation-level-prerequisite-installation.md) - Workstation-level installation strategy
