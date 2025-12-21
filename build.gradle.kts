// OpenXR Quest Tutorial - Root Build
// Unified build for all samples (OpenXR + Meta)
//
// Usage:
//   ./gradlew.bat assembleAllDebug    - Build all debug APKs
//   ./gradlew.bat assembleAllRelease  - Build all release APKs
//   ./gradlew.bat cleanAll            - Clean all samples

tasks.register("assembleAllDebug") {
    description = "Assembles debug APKs for all samples (OpenXR + Meta)"
    group = "build"

    // OpenXR samples
    dependsOn(gradle.includedBuild("openxr").task(":assembleAllDebug"))
    // Meta samples
    dependsOn(gradle.includedBuild("Samples").task(":assembleAllDebug"))
}

tasks.register("assembleAllRelease") {
    description = "Assembles release APKs for all samples (OpenXR + Meta)"
    group = "build"

    // OpenXR samples
    dependsOn(gradle.includedBuild("openxr").task(":assembleAllRelease"))
    // Meta samples
    dependsOn(gradle.includedBuild("Samples").task(":assembleAllRelease"))
}

tasks.register("cleanAll") {
    description = "Cleans all samples (OpenXR + Meta)"
    group = "build"

    // OpenXR samples
    dependsOn(gradle.includedBuild("openxr").task(":cleanAll"))
    // Meta samples
    dependsOn(gradle.includedBuild("Samples").task(":cleanAll"))
}

// Convenience aliases
tasks.register("buildAll") {
    description = "Alias for assembleAllDebug"
    group = "build"
    dependsOn("assembleAllDebug")
}
