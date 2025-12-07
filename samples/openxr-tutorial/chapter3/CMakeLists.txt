# OpenXR Tutorial Chapter 3 - GraphicsAPI & Swapchains
# Adapted from: https://openxr-tutorial.com/android/vulkan/3-graphics-api.html
# Modified for Quest 3 with Maven OpenXR loader + CMake FetchContent

cmake_minimum_required(VERSION 3.22.1)
project(openxr_tutorial_ch3)

set(CMAKE_CXX_STANDARD 17)
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

# Main application files
set(APP_HEADERS
    # Main headers will be added here
)

set(APP_SOURCE
    main.cpp
)

# Import android_native_app_glue from NDK
add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)
target_include_directories(native_app_glue PUBLIC
    ${ANDROID_NDK}/sources/android/native_app_glue
)

# Build as shared library for Android (SHARED, not MODULE, for NativeActivity)
add_library(openxr_tutorial_ch3 SHARED
    ${APP_SOURCE}
    ${APP_HEADERS}
    ${COMMON_SOURCE}
    ${COMMON_HEADERS}
)

# Android libraries
find_library(ANDROID_LIBRARY android)
find_library(ANDROID_LOG_LIBRARY log)

target_link_libraries(openxr_tutorial_ch3 PRIVATE
    ${ANDROID_LIBRARY}
    ${ANDROID_LOG_LIBRARY}
    native_app_glue
)

# Android platform defines
target_compile_definitions(openxr_tutorial_ch3 PRIVATE
    XR_USE_PLATFORM_ANDROID
    XR_USE_GRAPHICS_API_VULKAN
    XR_TUTORIAL_USE_VULKAN
    XR_TUTORIAL_GRAPHICS_API=VULKAN
)

# Include directories
target_include_directories(openxr_tutorial_ch3 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Common  # Shared Common directory
    ${openxr_BINARY_DIR}/include  # Generated OpenXR headers from FetchContent
)

# Link OpenXR headers from FetchContent and loader from Maven/Prefab
target_link_libraries(openxr_tutorial_ch3 PRIVATE
    OpenXR::headers  # Headers from FetchContent
    openxr_loader    # Library from Maven AAR via Prefab
)

# Vulkan support
find_library(VULKAN_LIBRARY vulkan)
if(VULKAN_LIBRARY)
    target_link_libraries(openxr_tutorial_ch3 PRIVATE ${VULKAN_LIBRARY})
else()
    message(WARNING "Vulkan library not found")
endif()

# Set linker flag for native activity
set_target_properties(openxr_tutorial_ch3 PROPERTIES
    LINK_FLAGS "-u ANativeActivity_onCreate"
)

# ============================================================================
# Shader Compilation (GLSL -> SPIR-V for Vulkan)
# ============================================================================

# Find glslc (SPIR-V compiler from Android NDK or Vulkan SDK)
# Determine host platform tag for NDK shader-tools
# NOTE: CMAKE_HOST_SYSTEM_NAME is the actual build machine OS (not target)
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(NDK_HOST_TAG "windows-x86_64")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(NDK_HOST_TAG "darwin-x86_64")
else()
    set(NDK_HOST_TAG "linux-x86_64")
endif()

# Get NDK path - try multiple sources in order of preference
# 1. CMAKE_ANDROID_NDK or ANDROID_NDK (set by Android Gradle Plugin)
# 2. ANDROID_HOME environment variable (documented in Environment_Setup.md)
if(DEFINED ANDROID_NDK)
    set(NDK_PATH ${ANDROID_NDK})
    message(STATUS "Using NDK from ANDROID_NDK: ${NDK_PATH}")
elseif(DEFINED CMAKE_ANDROID_NDK)
    set(NDK_PATH ${CMAKE_ANDROID_NDK})
    message(STATUS "Using NDK from CMAKE_ANDROID_NDK: ${NDK_PATH}")
elseif(DEFINED ENV{ANDROID_HOME})
    # Standard NDK location within Android SDK (NDK r27 as documented)
    set(NDK_PATH "$ENV{ANDROID_HOME}/ndk/27.2.12479018")
    if(EXISTS "${NDK_PATH}")
        message(STATUS "Using NDK from ANDROID_HOME: ${NDK_PATH}")
    else()
        message(WARNING "ANDROID_HOME is set but NDK not found at ${NDK_PATH}")
        set(NDK_PATH "")
    endif()
else()
    message(WARNING "Android NDK path not found. Set ANDROID_HOME environment variable (see docs/Environment_Setup.md)")
endif()

find_program(GLSLC_EXECUTABLE
    NAMES glslc glslc.exe
    PATHS
        "${NDK_PATH}/shader-tools/${NDK_HOST_TAG}"
        "$ENV{VULKAN_SDK}/bin"
    NO_DEFAULT_PATH
)

# Debug: Show what we're searching for
if(NOT GLSLC_EXECUTABLE AND NDK_PATH)
    message(WARNING "glslc search failed. Searched in: ${NDK_PATH}/shader-tools/${NDK_HOST_TAG}")
endif()

if(NOT GLSLC_EXECUTABLE)
    message(WARNING "glslc not found. Shaders will not be compiled. Install Vulkan SDK or use NDK shader-tools.")
else()
    message(STATUS "Found glslc: ${GLSLC_EXECUTABLE}")

    # Shader source files (from shared Shaders directory)
    set(VERTEX_SHADER ${CMAKE_CURRENT_SOURCE_DIR}/../Shaders/VertexShader.glsl)
    set(FRAGMENT_SHADER ${CMAKE_CURRENT_SOURCE_DIR}/../Shaders/PixelShader.glsl)

    # Output SPIR-V files (in build directory)
    set(VERTEX_SPV ${CMAKE_CURRENT_BINARY_DIR}/shaders/VertexShader.spv)
    set(FRAGMENT_SPV ${CMAKE_CURRENT_BINARY_DIR}/shaders/PixelShader.spv)

    # Create output directory
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders)

    # Compile vertex shader
    add_custom_command(
        OUTPUT ${VERTEX_SPV}
        COMMAND ${GLSLC_EXECUTABLE} -fshader-stage=vertex ${VERTEX_SHADER} -o ${VERTEX_SPV}
        DEPENDS ${VERTEX_SHADER}
        COMMENT "Compiling vertex shader: VertexShader.glsl -> VertexShader.spv"
        VERBATIM
    )

    # Compile fragment shader
    add_custom_command(
        OUTPUT ${FRAGMENT_SPV}
        COMMAND ${GLSLC_EXECUTABLE} -fshader-stage=fragment ${FRAGMENT_SHADER} -o ${FRAGMENT_SPV}
        DEPENDS ${FRAGMENT_SHADER}
        COMMENT "Compiling fragment shader: PixelShader.glsl -> PixelShader.spv"
        VERBATIM
    )

    # Create custom target that depends on shader compilation
    add_custom_target(compile_shaders ALL
        DEPENDS ${VERTEX_SPV} ${FRAGMENT_SPV}
    )

    # Make the main target depend on shader compilation
    add_dependencies(openxr_tutorial_ch3 compile_shaders)

    message(STATUS "Shader compilation enabled. Output: ${CMAKE_CURRENT_BINARY_DIR}/shaders/")
endif()
