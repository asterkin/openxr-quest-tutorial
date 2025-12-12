# Vulkan GLSL Shader Compilation for OpenXR Tutorial
# Include this from chapter CMakeLists.txt after Common/CMakeLists.txt
#
# Expects:
#   ${PROJECT_NAME} - target to add shader sources to
#   ${CMAKE_CURRENT_SOURCE_DIR} - chapter directory (for output path)
#
# Compiles GLSL shaders to SPIR-V and places them in app/src/main/assets/shaders/

# Add cmake module path and include shader compilation function
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")
include(glsl_shader)

# Shader source directory (this directory)
set(SHADER_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Output directory (chapter's assets folder)
set(SHADER_DEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/app/src/main/assets/shaders")

# Ensure output directory exists
file(MAKE_DIRECTORY "${SHADER_DEST_DIR}")

# Vulkan GLSL shaders to compile
set(GLSL_SHADERS
    "${SHADER_SRC_DIR}/VertexShader.glsl"
    "${SHADER_SRC_DIR}/PixelShader.glsl"
)

# Compile each shader to SPIR-V
foreach(SHADER ${GLSL_SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME_WE)

    # Determine shader stage from filename
    if(SHADER_NAME MATCHES "Vertex")
        set(SHADER_STAGE "vert")
    else()
        set(SHADER_STAGE "frag")
    endif()

    set(SHADER_OUTPUT "${SHADER_DEST_DIR}/${SHADER_NAME}.spv")

    glsl_spv_shader(
        INPUT "${SHADER}"
        OUTPUT "${SHADER_OUTPUT}"
        STAGE ${SHADER_STAGE}
        ENTRY_POINT main
        TARGET_ENV vulkan1.0
    )

    # Add compiled shader to target sources (ensures it gets built)
    target_sources(${PROJECT_NAME} PRIVATE "${SHADER_OUTPUT}")
endforeach()

message(STATUS "Shader compilation enabled for ${PROJECT_NAME}")
