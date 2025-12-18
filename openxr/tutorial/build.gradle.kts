// OpenXR Tutorial - Root Build
// Orchestrates building all tutorial chapters

tasks.register("assembleDebug") {
    description = "Assembles debug APKs for all tutorial chapters"
    group = "build"

    gradle.includedBuilds.forEach { includedBuild ->
        dependsOn(includedBuild.task(":app:assembleDebug"))
    }
}

tasks.register("assembleRelease") {
    description = "Assembles release APKs for all tutorial chapters"
    group = "build"

    gradle.includedBuilds.forEach { includedBuild ->
        dependsOn(includedBuild.task(":app:assembleRelease"))
    }
}

tasks.register("clean") {
    description = "Cleans all tutorial chapters"
    group = "build"

    gradle.includedBuilds.forEach { includedBuild ->
        dependsOn(includedBuild.task(":app:clean"))
    }
}
