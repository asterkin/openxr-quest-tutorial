// Copyright 2023, The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0

// OpenXR Tutorial for Khronos Group

#include <GraphicsAPI.h>

bool CheckGraphicsAPI_TypeIsValidForPlatform(GraphicsAPI_Type type) {
    return type == VULKAN;
}

const char *GetGraphicsAPIInstanceExtensionString(GraphicsAPI_Type type) {
    if (type == VULKAN) {
        return XR_KHR_VULKAN_ENABLE_EXTENSION_NAME;
    }
    std::cerr << "ERROR: Unknown Graphics API." << std::endl;
    DEBUG_BREAK;
    return nullptr;
}

// GraphicsAPI

int64_t GraphicsAPI::SelectColorSwapchainFormat(const std::vector<int64_t> &formats) {
    const std::vector<int64_t> &supportSwapchainFormats = GetSupportedColorSwapchainFormats();

    const std::vector<int64_t>::const_iterator &swapchainFormatIt = std::find_first_of(formats.begin(), formats.end(),
                                                                                       std::begin(supportSwapchainFormats), std::end(supportSwapchainFormats));
    if (swapchainFormatIt == formats.end()) {
        std::cout << "ERROR: Unable to find supported Color Swapchain Format" << std::endl;
        DEBUG_BREAK;
        return 0;
    }

    return *swapchainFormatIt;
}

int64_t GraphicsAPI::SelectDepthSwapchainFormat(const std::vector<int64_t> &formats) {
    const std::vector<int64_t> &supportSwapchainFormats = GetSupportedDepthSwapchainFormats();

    const std::vector<int64_t>::const_iterator &swapchainFormatIt = std::find_first_of(formats.begin(), formats.end(),
                                                                                       std::begin(supportSwapchainFormats), std::end(supportSwapchainFormats));
    if (swapchainFormatIt == formats.end()) {
        std::cout << "ERROR: Unable to find supported Depth Swapchain Format" << std::endl;
        DEBUG_BREAK;
        return 0;
    }

    return *swapchainFormatIt;
}
