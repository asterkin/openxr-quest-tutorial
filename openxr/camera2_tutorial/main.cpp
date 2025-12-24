// Copyright 2025, The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0

// OpenXR Camera2 Tutorial (standalone scaffold)

#include <DebugOutput.h>
#include <GraphicsAPI_Vulkan.h>
#include <OpenXRDebugUtils.h>

#include <algorithm>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

class Camera2Tutorial {
public:
    Camera2Tutorial()
        : m_apiType(VULKAN) {
        if (!CheckGraphicsAPI_TypeIsValidForPlatform(m_apiType)) {
            XR_TUT_LOG_ERROR("ERROR: Vulkan is not valid for this platform.");
            DEBUG_BREAK;
        }
    }
    ~Camera2Tutorial() = default;

    void Run() {
        CreateInstance();
        CreateDebugMessenger();
        GetInstanceProperties();
        GetSystemID();
        GetViewConfigurationViews();
        GetEnvironmentBlendModes();
        CreateSession();
        CreateReferenceSpace();
        CreateSwapchains();

        while (m_applicationRunning) {
            PollSystemEvents();
            PollEvents();
            if (m_sessionRunning) {
                RenderFrame();
            }
        }

        DestroySwapchains();
        DestroyReferenceSpace();
        DestroySession();
        DestroyDebugMessenger();
        DestroyInstance();
    }

private:
    void CreateInstance() {
        XrApplicationInfo appInfo{};
        std::strncpy(appInfo.applicationName, "OpenXR Camera2 Tutorial", XR_MAX_APPLICATION_NAME_SIZE);
        appInfo.applicationVersion = 1;
        std::strncpy(appInfo.engineName, "OpenXR Tutorial Engine", XR_MAX_ENGINE_NAME_SIZE);
        appInfo.engineVersion = 1;
        appInfo.apiVersion = XR_CURRENT_API_VERSION;

        m_instanceExtensions.push_back(XR_EXT_DEBUG_UTILS_EXTENSION_NAME);
        m_instanceExtensions.push_back(GetGraphicsAPIInstanceExtensionString(m_apiType));

        uint32_t apiLayerCount = 0;
        std::vector<XrApiLayerProperties> apiLayerProperties;
        OPENXR_CHECK(xrEnumerateApiLayerProperties(0, &apiLayerCount, nullptr), "Failed to enumerate ApiLayerProperties.");
        apiLayerProperties.resize(apiLayerCount, {XR_TYPE_API_LAYER_PROPERTIES});
        OPENXR_CHECK(xrEnumerateApiLayerProperties(apiLayerCount, &apiLayerCount, apiLayerProperties.data()), "Failed to enumerate ApiLayerProperties.");

        for (auto &requestLayer : m_apiLayers) {
            for (auto &layerProperty : apiLayerProperties) {
                if (strcmp(requestLayer.c_str(), layerProperty.layerName) != 0) {
                    continue;
                }
                m_activeAPILayers.push_back(requestLayer.c_str());
                break;
            }
        }

        uint32_t extensionCount = 0;
        std::vector<XrExtensionProperties> extensionProperties;
        OPENXR_CHECK(xrEnumerateInstanceExtensionProperties(nullptr, 0, &extensionCount, nullptr), "Failed to enumerate InstanceExtensionProperties.");
        extensionProperties.resize(extensionCount, {XR_TYPE_EXTENSION_PROPERTIES});
        OPENXR_CHECK(xrEnumerateInstanceExtensionProperties(nullptr, extensionCount, &extensionCount, extensionProperties.data()), "Failed to enumerate InstanceExtensionProperties.");

        for (auto &requestedInstanceExtension : m_instanceExtensions) {
            bool found = false;
            for (auto &extensionProperty : extensionProperties) {
                if (strcmp(requestedInstanceExtension.c_str(), extensionProperty.extensionName) != 0) {
                    continue;
                }
                m_activeInstanceExtensions.push_back(requestedInstanceExtension.c_str());
                found = true;
                break;
            }
            if (!found) {
                XR_TUT_LOG_ERROR("Failed to find OpenXR instance extension: " << requestedInstanceExtension);
            }
        }

        XrInstanceCreateInfo instanceCI{XR_TYPE_INSTANCE_CREATE_INFO};
        instanceCI.applicationInfo = appInfo;
        instanceCI.enabledApiLayerCount = static_cast<uint32_t>(m_activeAPILayers.size());
        instanceCI.enabledApiLayerNames = m_activeAPILayers.data();
        instanceCI.enabledExtensionCount = static_cast<uint32_t>(m_activeInstanceExtensions.size());
        instanceCI.enabledExtensionNames = m_activeInstanceExtensions.data();
        OPENXR_CHECK(xrCreateInstance(&instanceCI, &m_xrInstance), "Failed to create Instance.");
    }

    void DestroyInstance() {
        OPENXR_CHECK(xrDestroyInstance(m_xrInstance), "Failed to destroy Instance.");
    }

    void CreateDebugMessenger() {
        if (IsStringInVector(m_activeInstanceExtensions, XR_EXT_DEBUG_UTILS_EXTENSION_NAME)) {
            m_debugUtilsMessenger = CreateOpenXRDebugUtilsMessenger(m_xrInstance);
        }
    }

    void DestroyDebugMessenger() {
        if (m_debugUtilsMessenger != XR_NULL_HANDLE) {
            DestroyOpenXRDebugUtilsMessenger(m_xrInstance, m_debugUtilsMessenger);
        }
    }

    void GetInstanceProperties() {
        XrInstanceProperties instanceProperties{XR_TYPE_INSTANCE_PROPERTIES};
        OPENXR_CHECK(xrGetInstanceProperties(m_xrInstance, &instanceProperties), "Failed to get InstanceProperties.");

        XR_TUT_LOG("OpenXR Runtime: " << instanceProperties.runtimeName << " - "
                                      << XR_VERSION_MAJOR(instanceProperties.runtimeVersion) << "."
                                      << XR_VERSION_MINOR(instanceProperties.runtimeVersion) << "."
                                      << XR_VERSION_PATCH(instanceProperties.runtimeVersion));
    }

    void GetSystemID() {
        XrSystemGetInfo systemGI{XR_TYPE_SYSTEM_GET_INFO};
        systemGI.formFactor = m_formFactor;
        OPENXR_CHECK(xrGetSystem(m_xrInstance, &systemGI, &m_systemID), "Failed to get SystemID.");
        OPENXR_CHECK(xrGetSystemProperties(m_xrInstance, m_systemID, &m_systemProperties), "Failed to get SystemProperties.");
    }

    void GetViewConfigurationViews() {
        uint32_t viewConfigurationCount = 0;
        OPENXR_CHECK(xrEnumerateViewConfigurations(m_xrInstance, m_systemID, 0, &viewConfigurationCount, nullptr), "Failed to enumerate View Configurations.");
        m_viewConfigurations.resize(viewConfigurationCount);
        OPENXR_CHECK(xrEnumerateViewConfigurations(m_xrInstance, m_systemID, viewConfigurationCount, &viewConfigurationCount, m_viewConfigurations.data()),
                     "Failed to enumerate View Configurations.");

        for (const XrViewConfigurationType &viewConfiguration : m_applicationViewConfigurations) {
            if (std::find(m_viewConfigurations.begin(), m_viewConfigurations.end(), viewConfiguration) != m_viewConfigurations.end()) {
                m_viewConfiguration = viewConfiguration;
                break;
            }
        }
        if (m_viewConfiguration == XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM) {
            XR_TUT_LOG_ERROR("Failed to find a compatible view configuration. Defaulting to PRIMARY_STEREO.");
            m_viewConfiguration = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
        }

        uint32_t viewConfigurationViewCount = 0;
        OPENXR_CHECK(xrEnumerateViewConfigurationViews(m_xrInstance, m_systemID, m_viewConfiguration, 0, &viewConfigurationViewCount, nullptr),
                     "Failed to enumerate ViewConfiguration Views.");
        m_viewConfigurationViews.resize(viewConfigurationViewCount, {XR_TYPE_VIEW_CONFIGURATION_VIEW});
        OPENXR_CHECK(xrEnumerateViewConfigurationViews(m_xrInstance, m_systemID, m_viewConfiguration, viewConfigurationViewCount, &viewConfigurationViewCount,
                                                       m_viewConfigurationViews.data()),
                     "Failed to enumerate ViewConfiguration Views.");
    }

    void GetEnvironmentBlendModes() {
        uint32_t environmentBlendModeCount = 0;
        OPENXR_CHECK(xrEnumerateEnvironmentBlendModes(m_xrInstance, m_systemID, m_viewConfiguration, 0, &environmentBlendModeCount, nullptr),
                     "Failed to enumerate EnvironmentBlend Modes.");
        m_environmentBlendModes.resize(environmentBlendModeCount);
        OPENXR_CHECK(xrEnumerateEnvironmentBlendModes(m_xrInstance, m_systemID, m_viewConfiguration, environmentBlendModeCount, &environmentBlendModeCount,
                                                      m_environmentBlendModes.data()),
                     "Failed to enumerate EnvironmentBlend Modes.");

        for (const XrEnvironmentBlendMode &environmentBlendMode : m_applicationEnvironmentBlendModes) {
            if (std::find(m_environmentBlendModes.begin(), m_environmentBlendModes.end(), environmentBlendMode) != m_environmentBlendModes.end()) {
                m_environmentBlendMode = environmentBlendMode;
                break;
            }
        }
        if (m_environmentBlendMode == XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM) {
            XR_TUT_LOG_ERROR("Failed to find a compatible blend mode. Defaulting to OPAQUE.");
            m_environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
        }
    }

    void CreateSession() {
        m_graphicsAPI = std::make_unique<GraphicsAPI_Vulkan>(m_xrInstance, m_systemID);

        XrSessionCreateInfo sessionCI{XR_TYPE_SESSION_CREATE_INFO};
        sessionCI.next = m_graphicsAPI->GetGraphicsBinding();
        sessionCI.systemId = m_systemID;

        OPENXR_CHECK(xrCreateSession(m_xrInstance, &sessionCI, &m_session), "Failed to create Session.");
    }

    void DestroySession() {
        OPENXR_CHECK(xrDestroySession(m_session), "Failed to destroy Session.");
    }

    void CreateReferenceSpace() {
        XrReferenceSpaceCreateInfo referenceSpaceCI{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
        referenceSpaceCI.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
        referenceSpaceCI.poseInReferenceSpace = {{0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
        OPENXR_CHECK(xrCreateReferenceSpace(m_session, &referenceSpaceCI, &m_localSpace), "Failed to create ReferenceSpace.");
    }

    void DestroyReferenceSpace() {
        if (m_localSpace != XR_NULL_HANDLE) {
            OPENXR_CHECK(xrDestroySpace(m_localSpace), "Failed to destroy ReferenceSpace.");
            m_localSpace = XR_NULL_HANDLE;
        }
    }

    void CreateSwapchains() {
        uint32_t formatCount = 0;
        OPENXR_CHECK(xrEnumerateSwapchainFormats(m_session, 0, &formatCount, nullptr), "Failed to enumerate Swapchain Formats.");
        std::vector<int64_t> formats(formatCount);
        OPENXR_CHECK(xrEnumerateSwapchainFormats(m_session, formatCount, &formatCount, formats.data()), "Failed to enumerate Swapchain Formats.");

        const int64_t colorFormat = m_graphicsAPI->SelectColorSwapchainFormat(formats);

        m_colorSwapchainInfos.resize(m_viewConfigurationViews.size());
        for (size_t i = 0; i < m_viewConfigurationViews.size(); ++i) {
            const XrViewConfigurationView &view = m_viewConfigurationViews[i];
            SwapchainInfo &swapchainInfo = m_colorSwapchainInfos[i];

            XrSwapchainCreateInfo swapchainCI{XR_TYPE_SWAPCHAIN_CREATE_INFO};
            swapchainCI.usageFlags = XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT | XR_SWAPCHAIN_USAGE_SAMPLED_BIT;
            swapchainCI.format = colorFormat;
            swapchainCI.sampleCount = view.recommendedSwapchainSampleCount;
            swapchainCI.width = view.recommendedImageRectWidth;
            swapchainCI.height = view.recommendedImageRectHeight;
            swapchainCI.faceCount = 1;
            swapchainCI.arraySize = 1;
            swapchainCI.mipCount = 1;

            OPENXR_CHECK(xrCreateSwapchain(m_session, &swapchainCI, &swapchainInfo.swapchain), "Failed to create Color Swapchain.");
            swapchainInfo.swapchainFormat = colorFormat;

            uint32_t imageCount = 0;
            OPENXR_CHECK(xrEnumerateSwapchainImages(swapchainInfo.swapchain, 0, &imageCount, nullptr), "Failed to enumerate Swapchain Images.");
            XrSwapchainImageBaseHeader *images = m_graphicsAPI->AllocateSwapchainImageData(swapchainInfo.swapchain, GraphicsAPI::SwapchainType::COLOR, imageCount);
            OPENXR_CHECK(xrEnumerateSwapchainImages(swapchainInfo.swapchain, imageCount, &imageCount, images), "Failed to enumerate Swapchain Images.");

            swapchainInfo.imageViews.resize(imageCount);
            for (uint32_t imageIndex = 0; imageIndex < imageCount; ++imageIndex) {
                void *image = m_graphicsAPI->GetSwapchainImage(swapchainInfo.swapchain, imageIndex);
                GraphicsAPI::ImageViewCreateInfo imageViewCI{};
                imageViewCI.image = image;
                imageViewCI.type = GraphicsAPI::ImageViewCreateInfo::Type::RTV;
                imageViewCI.view = GraphicsAPI::ImageViewCreateInfo::View::TYPE_2D;
                imageViewCI.format = swapchainInfo.swapchainFormat;
                imageViewCI.aspect = GraphicsAPI::ImageViewCreateInfo::Aspect::COLOR_BIT;
                imageViewCI.baseMipLevel = 0;
                imageViewCI.levelCount = 1;
                imageViewCI.baseArrayLayer = 0;
                imageViewCI.layerCount = 1;
                swapchainInfo.imageViews[imageIndex] = m_graphicsAPI->CreateImageView(imageViewCI);
            }
        }
    }

    void DestroySwapchains() {
        for (SwapchainInfo &swapchainInfo : m_colorSwapchainInfos) {
            for (void *&imageView : swapchainInfo.imageViews) {
                m_graphicsAPI->DestroyImageView(imageView);
            }
            m_graphicsAPI->FreeSwapchainImageData(swapchainInfo.swapchain);
            OPENXR_CHECK(xrDestroySwapchain(swapchainInfo.swapchain), "Failed to destroy Color Swapchain.");
        }
        m_colorSwapchainInfos.clear();
    }

    struct RenderLayerInfo {
        XrTime predictedDisplayTime = 0;
        std::vector<XrCompositionLayerBaseHeader *> layers;
        XrCompositionLayerProjection layerProjection = {XR_TYPE_COMPOSITION_LAYER_PROJECTION};
        std::vector<XrCompositionLayerProjectionView> layerProjectionViews;
    };

    void RenderFrame() {
        XrFrameState frameState{XR_TYPE_FRAME_STATE};
        XrFrameWaitInfo frameWaitInfo{XR_TYPE_FRAME_WAIT_INFO};
        OPENXR_CHECK(xrWaitFrame(m_session, &frameWaitInfo, &frameState), "Failed to wait for XR Frame.");

        XrFrameBeginInfo frameBeginInfo{XR_TYPE_FRAME_BEGIN_INFO};
        OPENXR_CHECK(xrBeginFrame(m_session, &frameBeginInfo), "Failed to begin the XR Frame.");

        RenderLayerInfo renderLayerInfo;
        renderLayerInfo.predictedDisplayTime = frameState.predictedDisplayTime;

        bool sessionActive = (m_sessionState == XR_SESSION_STATE_SYNCHRONIZED || m_sessionState == XR_SESSION_STATE_VISIBLE ||
                              m_sessionState == XR_SESSION_STATE_FOCUSED);
        if (sessionActive && frameState.shouldRender) {
            if (RenderLayer(renderLayerInfo)) {
                renderLayerInfo.layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader *>(&renderLayerInfo.layerProjection));
            }
        }

        XrFrameEndInfo frameEndInfo{XR_TYPE_FRAME_END_INFO};
        frameEndInfo.displayTime = frameState.predictedDisplayTime;
        frameEndInfo.environmentBlendMode = m_environmentBlendMode;
        frameEndInfo.layerCount = static_cast<uint32_t>(renderLayerInfo.layers.size());
        frameEndInfo.layers = renderLayerInfo.layers.data();
        OPENXR_CHECK(xrEndFrame(m_session, &frameEndInfo), "Failed to end the XR Frame.");
    }

    bool RenderLayer(RenderLayerInfo &renderLayerInfo) {
        std::vector<XrView> views(m_viewConfigurationViews.size(), {XR_TYPE_VIEW});

        XrViewState viewState{XR_TYPE_VIEW_STATE};
        XrViewLocateInfo viewLocateInfo{XR_TYPE_VIEW_LOCATE_INFO};
        viewLocateInfo.viewConfigurationType = m_viewConfiguration;
        viewLocateInfo.displayTime = renderLayerInfo.predictedDisplayTime;
        viewLocateInfo.space = m_localSpace;
        uint32_t viewCount = 0;
        if (xrLocateViews(m_session, &viewLocateInfo, &viewState, static_cast<uint32_t>(views.size()), &viewCount, views.data()) != XR_SUCCESS) {
            XR_TUT_LOG("Failed to locate Views.");
            return false;
        }

        renderLayerInfo.layerProjectionViews.resize(viewCount, {XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW});

        for (uint32_t i = 0; i < viewCount; ++i) {
            SwapchainInfo &colorSwapchainInfo = m_colorSwapchainInfos[i];

            uint32_t colorImageIndex = 0;
            XrSwapchainImageAcquireInfo acquireInfo{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};
            OPENXR_CHECK(xrAcquireSwapchainImage(colorSwapchainInfo.swapchain, &acquireInfo, &colorImageIndex), "Failed to acquire Color Swapchain Image.");

            XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
            waitInfo.timeout = XR_INFINITE_DURATION;
            OPENXR_CHECK(xrWaitSwapchainImage(colorSwapchainInfo.swapchain, &waitInfo), "Failed to wait for Color Swapchain Image.");

            renderLayerInfo.layerProjectionViews[i] = {XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW};
            renderLayerInfo.layerProjectionViews[i].pose = views[i].pose;
            renderLayerInfo.layerProjectionViews[i].fov = views[i].fov;
            renderLayerInfo.layerProjectionViews[i].subImage.swapchain = colorSwapchainInfo.swapchain;
            renderLayerInfo.layerProjectionViews[i].subImage.imageRect.offset.x = 0;
            renderLayerInfo.layerProjectionViews[i].subImage.imageRect.offset.y = 0;
            renderLayerInfo.layerProjectionViews[i].subImage.imageRect.extent.width =
                static_cast<int32_t>(m_viewConfigurationViews[i].recommendedImageRectWidth);
            renderLayerInfo.layerProjectionViews[i].subImage.imageRect.extent.height =
                static_cast<int32_t>(m_viewConfigurationViews[i].recommendedImageRectHeight);
            renderLayerInfo.layerProjectionViews[i].subImage.imageArrayIndex = 0;

            m_graphicsAPI->BeginRendering();
            m_graphicsAPI->ClearColor(colorSwapchainInfo.imageViews[colorImageIndex], 0.0f, 0.0f, 0.0f, 1.0f);
            m_graphicsAPI->EndRendering();

            XrSwapchainImageReleaseInfo releaseInfo{XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
            OPENXR_CHECK(xrReleaseSwapchainImage(colorSwapchainInfo.swapchain, &releaseInfo), "Failed to release Color Swapchain Image.");
        }

        renderLayerInfo.layerProjection.layerFlags =
            XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT | XR_COMPOSITION_LAYER_CORRECT_CHROMATIC_ABERRATION_BIT;
        renderLayerInfo.layerProjection.space = m_localSpace;
        renderLayerInfo.layerProjection.viewCount = static_cast<uint32_t>(renderLayerInfo.layerProjectionViews.size());
        renderLayerInfo.layerProjection.views = renderLayerInfo.layerProjectionViews.data();

        return true;
    }

    void PollEvents() {
        XrEventDataBuffer eventData{XR_TYPE_EVENT_DATA_BUFFER};
        auto XrPollEvents = [&]() -> bool {
            eventData = {XR_TYPE_EVENT_DATA_BUFFER};
            return xrPollEvent(m_xrInstance, &eventData) == XR_SUCCESS;
        };

        while (XrPollEvents()) {
            switch (eventData.type) {
            case XR_TYPE_EVENT_DATA_EVENTS_LOST: {
                XrEventDataEventsLost *eventsLost = reinterpret_cast<XrEventDataEventsLost *>(&eventData);
                XR_TUT_LOG("OPENXR: Events Lost: " << eventsLost->lostEventCount);
                break;
            }
            case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING: {
                XrEventDataInstanceLossPending *instanceLossPending = reinterpret_cast<XrEventDataInstanceLossPending *>(&eventData);
                XR_TUT_LOG("OPENXR: Instance Loss Pending at: " << instanceLossPending->lossTime);
                m_sessionRunning = false;
                m_applicationRunning = false;
                break;
            }
            case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED: {
                XrEventDataSessionStateChanged *sessionStateChanged = reinterpret_cast<XrEventDataSessionStateChanged *>(&eventData);
                if (sessionStateChanged->session != m_session) {
                    XR_TUT_LOG("XrEventDataSessionStateChanged for unknown Session");
                    break;
                }

                if (sessionStateChanged->state == XR_SESSION_STATE_READY) {
                    XrSessionBeginInfo sessionBeginInfo{XR_TYPE_SESSION_BEGIN_INFO};
                    sessionBeginInfo.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
                    OPENXR_CHECK(xrBeginSession(m_session, &sessionBeginInfo), "Failed to begin Session.");
                    m_sessionRunning = true;
                }
                if (sessionStateChanged->state == XR_SESSION_STATE_STOPPING) {
                    OPENXR_CHECK(xrEndSession(m_session), "Failed to end Session.");
                    m_sessionRunning = false;
                }
                if (sessionStateChanged->state == XR_SESSION_STATE_EXITING) {
                    m_sessionRunning = false;
                    m_applicationRunning = false;
                }
                if (sessionStateChanged->state == XR_SESSION_STATE_LOSS_PENDING) {
                    m_sessionRunning = false;
                    m_applicationRunning = false;
                }
                m_sessionState = sessionStateChanged->state;
                break;
            }
            default: {
                break;
            }
            }
        }
    }

#if defined(__ANDROID__)
public:
    static android_app *androidApp;

    struct AndroidAppState {
        ANativeWindow *nativeWindow = nullptr;
        bool resumed = false;
    };
    static AndroidAppState androidAppState;

    static void AndroidAppHandleCmd(struct android_app *app, int32_t cmd) {
        AndroidAppState *appState = (AndroidAppState *)app->userData;

        switch (cmd) {
        case APP_CMD_START: {
            break;
        }
        case APP_CMD_RESUME: {
            appState->resumed = true;
            break;
        }
        case APP_CMD_PAUSE: {
            appState->resumed = false;
            break;
        }
        case APP_CMD_STOP: {
            break;
        }
        case APP_CMD_DESTROY: {
            appState->nativeWindow = nullptr;
            break;
        }
        case APP_CMD_INIT_WINDOW: {
            appState->nativeWindow = app->window;
            break;
        }
        case APP_CMD_TERM_WINDOW: {
            appState->nativeWindow = nullptr;
            break;
        }
        }
    }

private:
    void PollSystemEvents() {
        if (androidApp->destroyRequested != 0) {
            m_applicationRunning = false;
            return;
        }
        while (true) {
            struct android_poll_source *source = nullptr;
            int events = 0;
            const int timeoutMilliseconds = (!androidAppState.resumed && !m_sessionRunning && androidApp->destroyRequested == 0) ? -1 : 0;
            if (ALooper_pollOnce(timeoutMilliseconds, nullptr, &events, (void **)&source) >= 0) {
                if (source != nullptr) {
                    source->process(androidApp, source);
                }
            } else {
                break;
            }
        }
    }
#else
    void PollSystemEvents() {
        return;
    }
#endif

private:
    XrInstance m_xrInstance = XR_NULL_HANDLE;
    std::vector<const char *> m_activeAPILayers = {};
    std::vector<const char *> m_activeInstanceExtensions = {};
    std::vector<std::string> m_apiLayers = {};
    std::vector<std::string> m_instanceExtensions = {};

    XrDebugUtilsMessengerEXT m_debugUtilsMessenger = XR_NULL_HANDLE;

    XrFormFactor m_formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
    XrSystemId m_systemID = {};
    XrSystemProperties m_systemProperties{XR_TYPE_SYSTEM_PROPERTIES};

    GraphicsAPI_Type m_apiType = UNKNOWN;
    std::unique_ptr<GraphicsAPI> m_graphicsAPI = nullptr;

    XrSession m_session = XR_NULL_HANDLE;
    XrSessionState m_sessionState = XR_SESSION_STATE_UNKNOWN;
    bool m_applicationRunning = true;
    bool m_sessionRunning = false;

    std::vector<XrViewConfigurationType> m_applicationViewConfigurations = {
        XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
        XR_VIEW_CONFIGURATION_TYPE_PRIMARY_MONO};
    std::vector<XrViewConfigurationType> m_viewConfigurations;
    XrViewConfigurationType m_viewConfiguration = XR_VIEW_CONFIGURATION_TYPE_MAX_ENUM;
    std::vector<XrViewConfigurationView> m_viewConfigurationViews;

    struct SwapchainInfo {
        XrSwapchain swapchain = XR_NULL_HANDLE;
        int64_t swapchainFormat = 0;
        std::vector<void *> imageViews;
    };
    std::vector<SwapchainInfo> m_colorSwapchainInfos = {};

    std::vector<XrEnvironmentBlendMode> m_applicationEnvironmentBlendModes = {XR_ENVIRONMENT_BLEND_MODE_OPAQUE};
    std::vector<XrEnvironmentBlendMode> m_environmentBlendModes = {};
    XrEnvironmentBlendMode m_environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM;

    XrSpace m_localSpace = XR_NULL_HANDLE;
};

void Camera2Tutorial_Main() {
    DebugOutput debugOutput;
    XR_TUT_LOG("OpenXR Camera2 Tutorial");

    Camera2Tutorial app;
    app.Run();
}

#if defined(_WIN32) || (defined(__linux__) && !defined(__ANDROID__))
int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    Camera2Tutorial_Main();
}
#elif (__ANDROID__)
android_app *Camera2Tutorial::androidApp = nullptr;
Camera2Tutorial::AndroidAppState Camera2Tutorial::androidAppState = {};

void android_main(struct android_app *app) {
    JNIEnv *env;
    app->activity->vm->AttachCurrentThread(&env, nullptr);

    XrInstance m_xrInstance = XR_NULL_HANDLE;
    PFN_xrInitializeLoaderKHR xrInitializeLoaderKHR = nullptr;
    OPENXR_CHECK(xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrInitializeLoaderKHR", (PFN_xrVoidFunction *)&xrInitializeLoaderKHR),
                 "Failed to get InstanceProcAddr for xrInitializeLoaderKHR.");
    if (!xrInitializeLoaderKHR) {
        return;
    }

    XrLoaderInitInfoAndroidKHR loaderInitializeInfoAndroid{XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR};
    loaderInitializeInfoAndroid.applicationVM = app->activity->vm;
    loaderInitializeInfoAndroid.applicationContext = app->activity->clazz;
    OPENXR_CHECK(xrInitializeLoaderKHR((XrLoaderInitInfoBaseHeaderKHR *)&loaderInitializeInfoAndroid),
                 "Failed to initialize Loader for Android.");

    app->userData = &Camera2Tutorial::androidAppState;
    app->onAppCmd = Camera2Tutorial::AndroidAppHandleCmd;

    Camera2Tutorial::androidApp = app;
    Camera2Tutorial_Main();
}
#endif
