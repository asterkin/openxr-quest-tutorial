# OpenXR Tutorial Chapter 1 - Introduction & Setup
# Adapted from: https://openxr-tutorial.com/android/vulkan/1-introduction.html
# Modified for Quest 3 with Maven OpenXR loader + CMake FetchContent

cmake_minimum_required(VERSION 3.22.1)
project("${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch OpenXR SDK headers via FetchContent (same pattern as hello_xr)
include(FetchContent)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_CONFORMANCE_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_API_LAYERS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    OpenXR
    EXCLUDE_FROM_ALL
    URL https://github.com/KhronosGroup/OpenXR-SDK-Source/archive/refs/tags/release-1.1.54.tar.gz
    SOURCE_DIR openxr
)
FetchContent_MakeAvailable(OpenXR)

# Common utility files (shared across all tutorial chapters)
set(COMMON_HEADERS
    ../Common/DebugOutput.h
    ../Common/HelperFunctions.h
    ../Common/OpenXRDebugUtils.h
    ../Common/OpenXRHelper.h
    ../Common/GraphicsAPI.h
    ../Common/GraphicsAPI_Vulkan.h
)

set(COMMON_SOURCE
    ../Common/OpenXRDebugUtils.cpp
    ../Common/GraphicsAPI.cpp
    ../Common/GraphicsAPI_Vulkan.cpp
)

# Import android_native_app_glue from NDK
add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)
target_include_directories(native_app_glue PUBLIC
    ${ANDROID_NDK}/sources/android/native_app_glue
)

# Build as shared library for Android (SHARED, not MODULE, for NativeActivity)
add_library(${PROJECT_NAME} SHARED
    ${APP_SOURCE}
    ${APP_HEADERS}
    ${COMMON_SOURCE}
    ${COMMON_HEADERS}
)

# Android libraries
find_library(ANDROID_LIBRARY android)
find_library(ANDROID_LOG_LIBRARY log)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${ANDROID_LIBRARY}
    ${ANDROID_LOG_LIBRARY}
    native_app_glue
)

# Android platform defines
target_compile_definitions(${PROJECT_NAME} PRIVATE
    XR_USE_PLATFORM_ANDROID
    XR_USE_GRAPHICS_API_VULKAN
    XR_TUTORIAL_USE_VULKAN
    XR_TUTORIAL_GRAPHICS_API=VULKAN
)

# Suppress cast-calling-convention warnings (matches original tutorial CMakeLists)
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-cast-calling-convention)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Common  # Shared Common directory
    ${openxr_BINARY_DIR}/include  # Generated OpenXR headers from FetchContent
)

# Link OpenXR headers from FetchContent and loader from Maven/Prefab
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenXR::headers  # Headers from FetchContent
    openxr_loader    # Library from Maven AAR via Prefab
)

# Vulkan support
find_library(VULKAN_LIBRARY vulkan)
if(VULKAN_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${VULKAN_LIBRARY})
else()
    message(WARNING "Vulkan library not found")
endif()

# Set linker flag for native activity
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "-u ANativeActivity_onCreate"
)
