// OpenXR Samples - Root Build
// Orchestrates building all OpenXR samples
//
// Usage:
//   ./gradlew.bat assembleAllDebug    - Build all debug APKs
//   ./gradlew.bat assembleAllRelease  - Build all release APKs
//   ./gradlew.bat cleanAll            - Clean all samples

// Standardized task names (for unified root build)
tasks.register("assembleAllDebug") {
    description = "Assembles debug APKs for all OpenXR samples"
    group = "build"

    dependsOn(gradle.includedBuild("hello_world").task(":assembleDebug"))
    dependsOn(gradle.includedBuild("hello_xr").task(":assembleVulkanDebug"))
    dependsOn(gradle.includedBuild("tutorial").task(":assembleDebug"))
}

tasks.register("assembleAllRelease") {
    description = "Assembles release APKs for all OpenXR samples"
    group = "build"

    dependsOn(gradle.includedBuild("hello_world").task(":assembleRelease"))
    dependsOn(gradle.includedBuild("hello_xr").task(":assembleVulkanRelease"))
    dependsOn(gradle.includedBuild("tutorial").task(":assembleRelease"))
}

tasks.register("cleanAll") {
    description = "Cleans all OpenXR samples"
    group = "build"

    dependsOn(gradle.includedBuild("hello_world").task(":clean"))
    dependsOn(gradle.includedBuild("hello_xr").task(":clean"))
    dependsOn(gradle.includedBuild("tutorial").task(":clean"))
}

// Legacy task aliases (for backwards compatibility)
tasks.register("assembleDebug") {
    description = "Alias for assembleAllDebug"
    group = "build"
    dependsOn("assembleAllDebug")
}

tasks.register("assembleRelease") {
    description = "Alias for assembleAllRelease"
    group = "build"
    dependsOn("assembleAllRelease")
}

tasks.register("clean") {
    description = "Alias for cleanAll"
    group = "build"
    dependsOn("cleanAll")
}
