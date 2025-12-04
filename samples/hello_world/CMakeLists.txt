cmake_minimum_required(VERSION 3.22.1)
project(hello_world)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch OpenXR SDK for headers and utilities
include(FetchContent)
FetchContent_Declare(
    OpenXR
    URL https://github.com/KhronosGroup/OpenXR-SDK-Source/archive/refs/tags/release-1.1.54.tar.gz
)
FetchContent_MakeAvailable(OpenXR)

# Import android_native_app_glue
add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)
target_include_directories(native_app_glue PUBLIC
    ${ANDROID_NDK}/sources/android/native_app_glue
)

# Build hello_world as a shared library for Android
add_library(hello_world SHARED
    main.cpp
)

# Android libraries
find_library(ANDROID_LIBRARY android)
find_library(ANDROID_LOG_LIBRARY log)

# Link libraries
target_link_libraries(hello_world
    android
    native_app_glue
    openxr_loader
    ${ANDROID_LOG_LIBRARY}
)

# Platform defines
target_compile_definitions(hello_world PRIVATE
    XR_USE_PLATFORM_ANDROID
)

# Set linker flag for native activity
set_target_properties(hello_world PROPERTIES
    LINK_FLAGS "-u ANativeActivity_onCreate"
)
