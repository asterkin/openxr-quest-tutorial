// OpenXR Samples - Root Build
// Orchestrates building all OpenXR samples

tasks.register("assembleDebug") {
    description = "Assembles debug APKs for all OpenXR samples"
    group = "build"

    dependsOn(gradle.includedBuild("hello_world").task(":assembleDebug"))
    dependsOn(gradle.includedBuild("hello_xr").task(":assembleVulkanDebug"))
    dependsOn(gradle.includedBuild("tutorial").task(":assembleDebug"))
}

tasks.register("assembleRelease") {
    description = "Assembles release APKs for all OpenXR samples"
    group = "build"

    dependsOn(gradle.includedBuild("hello_world").task(":assembleRelease"))
    dependsOn(gradle.includedBuild("hello_xr").task(":assembleVulkanRelease"))
    dependsOn(gradle.includedBuild("tutorial").task(":assembleRelease"))
}

tasks.register("clean") {
    description = "Cleans all OpenXR samples"
    group = "build"

    dependsOn(gradle.includedBuild("hello_world").task(":clean"))
    dependsOn(gradle.includedBuild("hello_xr").task(":clean"))
    dependsOn(gradle.includedBuild("tutorial").task(":clean"))
}
