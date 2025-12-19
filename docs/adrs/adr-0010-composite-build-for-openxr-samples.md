# ADR-0010: Composite Build for OpenXR Samples

**Status:** Accepted

**Date:** 2025-12-19

## Context

This repository contains multiple independent OpenXR samples, each a standalone Gradle project:
- `openxr/hello_world/` - Minimal OpenXR sample
- `openxr/hello_xr/` - Full Khronos reference sample (Vulkan/OpenGLES flavors)
- `openxr/tutorial/Chapter1-6/` - Progressive tutorial series (6 chapters)

Each sample can be built independently, which is valuable for learning and experimentation. However, this independence creates challenges:

1. **Tool Version Verification**: After upgrading NDK, AGP, or C++ versions, each sample must be built separately to verify compatibility
2. **CI/CD Complexity**: Build pipelines would need to enumerate and build each project individually
3. **Developer Experience**: New contributors must navigate to each directory to build samples

The repository needs a mechanism to build all samples from a single command while preserving standalone capability.

## Decision

We will **implement Gradle composite builds** using `includeBuild()` to orchestrate all OpenXR samples from a single entry point.

### Architecture

```
openxr/
├── gradlew.bat              ← Single entry point for all samples
├── settings.gradle.kts      ← Composite build configuration
├── build.gradle.kts         ← Orchestration tasks
├── hello_world/
│   ├── gradlew.bat          ← Standalone build preserved
│   └── build.gradle
├── hello_xr/
│   ├── gradlew.bat          ← Standalone build preserved
│   └── build.gradle
└── tutorial/
    ├── gradlew.bat          ← Builds all 6 chapters
    ├── settings.gradle.kts  ← Nested composite build
    ├── build.gradle.kts
    └── Chapter1-6/
        └── (standalone projects)
```

### Implementation

**Root settings.gradle.kts** (`openxr/`):
```kotlin
pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}
rootProject.name = "openxr-samples"

includeBuild("hello_world")
includeBuild("hello_xr")
includeBuild("tutorial")
```

**Root build.gradle.kts** (`openxr/`):
```kotlin
tasks.register("assembleDebug") {
    description = "Assembles debug APKs for all OpenXR samples"
    group = "build"

    dependsOn(gradle.includedBuild("hello_world").task(":assembleDebug"))
    dependsOn(gradle.includedBuild("hello_xr").task(":assembleVulkanDebug"))
    dependsOn(gradle.includedBuild("tutorial").task(":assembleDebug"))
}
```

**Tutorial nested composite** (`openxr/tutorial/settings.gradle.kts`):
```kotlin
rootProject.name = "openxr-tutorial"

(1..6).forEach { chapter ->
    includeBuild("Chapter$chapter")
}
```

### Alternatives Considered

**Option 1: Multi-module Project (include) - Rejected**
- Would require restructuring all samples into submodules
- Breaks standalone sample usage for learners
- Couples samples that should remain independent

**Option 2: Shell Script Orchestration - Rejected**
- Platform-specific scripts needed (batch, bash)
- No parallel execution benefits
- Doesn't leverage Gradle's dependency management

**Option 3: Gradle `buildSrc` with Shared Configuration - Rejected**
- Adds complexity inappropriate for tutorial repository
- Reduces sample independence
- Harder for beginners to understand

## Consequences

### Positive
- **Single Command Build**: `.\gradlew.bat assembleDebug` builds all samples
- **Parallel Execution**: Gradle automatically parallelizes independent builds
- **Version Verification**: Tool upgrades can be verified across all samples at once
- **CI/CD Ready**: Single entry point simplifies pipeline configuration
- **Preserved Independence**: Each sample remains buildable standalone

### Negative
- **Nested Complexity**: Tutorial uses nested composite build (openxr → tutorial → chapters)
- **Task Name Awareness**: Must know `hello_xr` uses `:assembleVulkanDebug` not `:assembleDebug`
- **Additional Files**: Root directories need settings.gradle.kts and build.gradle.kts

### Neutral
- Composite builds are a standard Gradle pattern, well-documented
- No changes required to individual sample build files
- Works seamlessly with automatic Gradle wrapper bootstrapping (ADR-0009)

## References

- [Gradle Composite Builds Documentation](https://docs.gradle.org/current/userguide/composite_builds.html)
- [ADR-0009: Automatic Gradle Wrapper Bootstrapping](adr-0009-automatic-gradle-wrapper-bootstrapping.md)
- [ADR-0007: Build and Run Architecture](adr-0007-build-and-run-architecture.md)
- [Project Plan - Phase 6: Upgrade to Latest Tool Versions](../Project_Plan.md)
