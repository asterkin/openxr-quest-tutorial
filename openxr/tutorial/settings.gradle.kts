// OpenXR Tutorial - Root Project Settings
// Composite build for all tutorial chapters

pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

rootProject.name = "openxr-tutorial"

// Include all chapters as composite builds
(1..6).forEach { chapter ->
    includeBuild("Chapter$chapter")
}
