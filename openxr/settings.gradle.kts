// OpenXR Samples - Root Project Settings
// Composite build for all OpenXR samples

pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

rootProject.name = "openxr-samples"

// Include all samples as composite builds
includeBuild("hello_world")
includeBuild("hello_xr")
includeBuild("tutorial")
includeBuild("camera2_tutorial")
