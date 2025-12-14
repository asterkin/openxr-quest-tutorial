cmake_minimum_required(VERSION 3.22.1)
project("${PROJECT_NAME}")

# Enable multiview rendering for Chapter 6
set(XR_TUTORIAL_ENABLE_MULTIVIEW ON)
add_compile_definitions(XR_TUTORIAL_ENABLE_MULTIVIEW=1)

# Main application files
set(APP_HEADERS
    # Main headers will be added here
)

set(APP_SOURCE
    main.cpp
)
include("../Common/CMakeLists.txt")
include("../Shaders/CMakeLists.txt")
