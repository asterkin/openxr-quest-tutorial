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

# Main application files
set(APP_HEADERS
    # Main headers will be added here
)

set(APP_SOURCE
    app/src/main/cpp/main.cpp
)

# Common utility files (Vulkan-only for this tutorial)
set(COMMON_HEADERS
    app/src/main/cpp/DebugOutput.h
    app/src/main/cpp/HelperFunctions.h
    app/src/main/cpp/OpenXRDebugUtils.h
    app/src/main/cpp/OpenXRHelper.h
    app/src/main/cpp/GraphicsAPI.h
    app/src/main/cpp/GraphicsAPI_Vulkan.h
)

set(COMMON_SOURCE
    app/src/main/cpp/OpenXRDebugUtils.cpp
    app/src/main/cpp/GraphicsAPI.cpp
    app/src/main/cpp/GraphicsAPI_Vulkan.cpp
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
find_library(ANDROID_CAMERA2_LIBRARY camera2ndk)
find_library(ANDROID_MEDIA_LIBRARY mediandk)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${ANDROID_LIBRARY}
    ${ANDROID_LOG_LIBRARY}
    ${ANDROID_CAMERA2_LIBRARY}
    ${ANDROID_MEDIA_LIBRARY}
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
    ${CMAKE_CURRENT_SOURCE_DIR}/app/src/main/cpp
    ${openxr_BINARY_DIR}/include
)

# Link OpenXR headers from FetchContent and loader from Maven/Prefab
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenXR::headers
    openxr_loader
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
