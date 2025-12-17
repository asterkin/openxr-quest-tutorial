# Copyright (c) 2017-2025 The Khronos Group Inc.
# SPDX-License-Identifier: Apache-2.0
# Modified for standalone build with Maven OpenXR loader + CMake FetchContent

cmake_minimum_required(VERSION 3.22.1)
project(hello_xr)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch OpenXR SDK for both headers and loader (same as hello_world)
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

# Source files
set(LOCAL_HEADERS
    check.h
    common.h
    geometry.h
    graphicsapi.h
    graphicsplugin.h
    logger.h
    openxr_program.h
    options.h
    pch.h
    platformdata.h
    platformplugin.h
    common/gfxwrapper_opengl.h
)

set(LOCAL_SOURCE
    graphicsplugin_factory.cpp
    graphicsplugin_opengles.cpp
    graphicsplugin_vulkan.cpp
    logger.cpp
    main.cpp
    openxr_program.cpp
    pch.cpp
    platformplugin_android.cpp
    platformplugin_factory.cpp
    glad/src/egl.c
    glad/src/gl.c
    common/gfxwrapper_opengl.c
)

set(VULKAN_SHADERS
    vulkan_shaders/frag.glsl
    vulkan_shaders/vert.glsl
)

# Convert precompiled Vulkan shaders from binary to C hex array format
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vert.spv
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/convert_spv_to_hex.py
            ${CMAKE_CURRENT_SOURCE_DIR}/vulkan_shaders/vert.spv
            ${CMAKE_CURRENT_BINARY_DIR}/vert.spv
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/vulkan_shaders/vert.spv
            ${CMAKE_CURRENT_SOURCE_DIR}/convert_spv_to_hex.py
    COMMENT "Converting vert.spv to C hex array"
)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/frag.spv
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/convert_spv_to_hex.py
            ${CMAKE_CURRENT_SOURCE_DIR}/vulkan_shaders/frag.spv
            ${CMAKE_CURRENT_BINARY_DIR}/frag.spv
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/vulkan_shaders/frag.spv
            ${CMAKE_CURRENT_SOURCE_DIR}/convert_spv_to_hex.py
    COMMENT "Converting frag.spv to C hex array"
)

# Add shader conversions as dependencies
add_custom_target(convert_shaders ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/vert.spv ${CMAKE_CURRENT_BINARY_DIR}/frag.spv
)

# Import android_native_app_glue from NDK
find_library(NATIVE_APP_GLUE_LIBRARY native_app_glue PATHS ${ANDROID_NDK}/sources/android/native_app_glue)
add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)
target_include_directories(native_app_glue PUBLIC
    ${ANDROID_NDK}/sources/android/native_app_glue
)

# Build hello_xr as a shared library for Android
add_library(hello_xr MODULE
    ${LOCAL_SOURCE}
    ${LOCAL_HEADERS}
    ${VULKAN_SHADERS}
)

# Ensure shaders are converted before compiling
add_dependencies(hello_xr convert_shaders)


# Android libraries
find_library(ANDROID_LIBRARY android)
find_library(ANDROID_LOG_LIBRARY log)

target_link_libraries(hello_xr PRIVATE
    ${ANDROID_LIBRARY}
    ${ANDROID_LOG_LIBRARY}
    native_app_glue
)

# Android platform defines
target_compile_definitions(hello_xr PRIVATE
    XR_USE_PLATFORM_ANDROID
    XR_USE_GRAPHICS_API_OPENGL_ES
    XR_USE_GRAPHICS_API_VULKAN
    GLAD_GLES2  # Enable OpenGL ES 2.0+ loader in GLAD
)

# Graphics plugin selection (set by Gradle)
set(HELLOXR_DEFAULT_GRAPHICS_PLUGIN "Vulkan" CACHE STRING
    "Which graphics plugin should be used by default?")
set_property(CACHE HELLOXR_DEFAULT_GRAPHICS_PLUGIN PROPERTY STRINGS "OpenGLES" "Vulkan")

if(HELLOXR_DEFAULT_GRAPHICS_PLUGIN STREQUAL "OpenGLES")
    message(STATUS "hello_xr will default to OpenGL ES graphics plugin")
    target_compile_definitions(hello_xr PRIVATE DEFAULT_GRAPHICS_PLUGIN_OPENGLES)
elseif(HELLOXR_DEFAULT_GRAPHICS_PLUGIN STREQUAL "Vulkan")
    message(STATUS "hello_xr will default to Vulkan graphics plugin")
    target_compile_definitions(hello_xr PRIVATE DEFAULT_GRAPHICS_PLUGIN_VULKAN)
endif()

target_include_directories(hello_xr PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
    ${openxr_BINARY_DIR}/include  # Generated OpenXR headers from FetchContent
)

# Link OpenXR headers from FetchContent and loader from Maven/Prefab
target_link_libraries(hello_xr PRIVATE
    OpenXR::headers  # Headers from FetchContent
    openxr_loader    # Library from Maven AAR via Prefab
)

# Vulkan support
if(HELLOXR_DEFAULT_GRAPHICS_PLUGIN STREQUAL "Vulkan")
    find_library(VULKAN_LIBRARY vulkan)
    if(VULKAN_LIBRARY)
        target_link_libraries(hello_xr PRIVATE ${VULKAN_LIBRARY})
    endif()
endif()

# OpenGL ES support
if(HELLOXR_DEFAULT_GRAPHICS_PLUGIN STREQUAL "OpenGLES")
    find_library(GLES_LIBRARY GLESv3)
    find_library(EGL_LIBRARY EGL)
    if(GLES_LIBRARY AND EGL_LIBRARY)
        target_link_libraries(hello_xr PRIVATE ${GLES_LIBRARY} ${EGL_LIBRARY})
    endif()
endif()

# Set linker flag for native activity
set_target_properties(hello_xr PROPERTIES
    LINK_FLAGS "-u ANativeActivity_onCreate"
)
