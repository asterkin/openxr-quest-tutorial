// OpenXR Quest Tutorial - Root Project Settings
// Unified composite build for all samples (OpenXR + Meta)

pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

rootProject.name = "openxr-quest-tutorial"

// Include both sample collections as composite builds
includeBuild("openxr")
includeBuild("meta/Samples")
