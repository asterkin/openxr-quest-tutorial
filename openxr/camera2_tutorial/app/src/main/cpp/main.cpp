// Copyright 2025, The Khronos Group Inc.
//
// SPDX-License-Identifier: Apache-2.0

// OpenXR Camera2 Tutorial (standalone scaffold)

#include <DebugOutput.h>
#include <GraphicsAPI_Vulkan.h>
#include <OpenXRDebugUtils.h>

#include <atomic>
#include <algorithm>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <memory>
#include <string>
#include <vector>

#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraMetadata.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCaptureRequest.h>
#include <media/NdkImageReader.h>

static std::atomic<bool> g_cameraPermissionGranted{false};

static uint8_t ClampToByte(int value) {
    if (value < 0) {
        return 0;
    }
    if (value > 255) {
        return 255;
    }
    return static_cast<uint8_t>(value);
}

static bool IsBgraFormat(int64_t format) {
    return format == VK_FORMAT_B8G8R8A8_UNORM || format == VK_FORMAT_B8G8R8A8_SRGB;
}

static void SwizzleRgbaToBgra(std::vector<uint8_t> &rgba) {
    for (size_t i = 0; i + 3 < rgba.size(); i += 4) {
        std::swap(rgba[i + 0], rgba[i + 2]);
    }
}

class Camera2Capture {
public:
    bool Start(uint32_t desiredWidth, uint32_t desiredHeight) {
        if (m_running) {
            return true;
        }

        auto fail = [&](const char *message) -> bool {
            XR_TUT_LOG_ERROR(message);
            Stop();
            return false;
        };

        m_outputWidth = desiredWidth;
        m_outputHeight = desiredHeight;

        m_cameraManager = ACameraManager_create();
        if (!m_cameraManager) {
            return fail("Camera2: Failed to create camera manager.");
        }

        ACameraIdList *cameraIdList = nullptr;
        if (ACameraManager_getCameraIdList(m_cameraManager, &cameraIdList) != ACAMERA_OK || !cameraIdList) {
            return fail("Camera2: Failed to get camera id list.");
        }

        m_cameraId = SelectCameraId(cameraIdList);
        if (m_cameraId.empty()) {
            ACameraManager_deleteCameraIdList(cameraIdList);
            return fail("Camera2: No suitable camera found.");
        }

        ACameraManager_deleteCameraIdList(cameraIdList);
        XR_TUT_LOG("Camera2: Selected camera id " << m_cameraId);

        if (!SelectCameraSize(m_cameraId.c_str(), desiredWidth, desiredHeight)) {
            return fail("Camera2: Failed to select camera size.");
        }

        if (AImageReader_new(m_cameraWidth, m_cameraHeight, AIMAGE_FORMAT_YUV_420_888, 2, &m_imageReader) != AMEDIA_OK) {
            return fail("Camera2: Failed to create image reader.");
        }

        if (AImageReader_getWindow(m_imageReader, &m_imageReaderWindow) != AMEDIA_OK) {
            return fail("Camera2: Failed to get image reader window.");
        }

        m_imageListener.context = this;
        m_imageListener.onImageAvailable = Camera2Capture::OnImageAvailable;
        if (AImageReader_setImageListener(m_imageReader, &m_imageListener) != AMEDIA_OK) {
            return fail("Camera2: Failed to set image listener.");
        }

        m_deviceCallbacks.context = this;
        m_deviceCallbacks.onDisconnected = Camera2Capture::OnCameraDisconnected;
        m_deviceCallbacks.onError = Camera2Capture::OnCameraError;
        if (ACameraManager_openCamera(m_cameraManager, m_cameraId.c_str(), &m_deviceCallbacks, &m_cameraDevice) != ACAMERA_OK) {
            return fail("Camera2: Failed to open camera.");
        }

        if (ACameraDevice_createCaptureRequest(m_cameraDevice, TEMPLATE_PREVIEW, &m_captureRequest) != ACAMERA_OK) {
            return fail("Camera2: Failed to create capture request.");
        }

        if (ACameraOutputTarget_create(m_imageReaderWindow, &m_outputTarget) != ACAMERA_OK) {
            return fail("Camera2: Failed to create output target.");
        }

        if (ACaptureRequest_addTarget(m_captureRequest, m_outputTarget) != ACAMERA_OK) {
            return fail("Camera2: Failed to add output target.");
        }

        if (ACaptureSessionOutput_create(m_imageReaderWindow, &m_sessionOutput) != ACAMERA_OK) {
            return fail("Camera2: Failed to create session output.");
        }

        if (ACaptureSessionOutputContainer_create(&m_outputContainer) != ACAMERA_OK) {
            return fail("Camera2: Failed to create output container.");
        }

        if (ACaptureSessionOutputContainer_add(m_outputContainer, m_sessionOutput) != ACAMERA_OK) {
            return fail("Camera2: Failed to add session output to container.");
        }

        m_sessionCallbacks.context = this;
        m_sessionCallbacks.onClosed = Camera2Capture::OnSessionClosed;
        m_sessionCallbacks.onReady = Camera2Capture::OnSessionReady;
        m_sessionCallbacks.onActive = Camera2Capture::OnSessionActive;
        if (ACameraDevice_createCaptureSession(m_cameraDevice, m_outputContainer, &m_sessionCallbacks, &m_captureSession) != ACAMERA_OK) {
            return fail("Camera2: Failed to create capture session.");
        }

        int sequenceId = 0;
        if (ACameraCaptureSession_setRepeatingRequest(m_captureSession, nullptr, 1, &m_captureRequest, &sequenceId) != ACAMERA_OK) {
            return fail("Camera2: Failed to start repeating request.");
        }

        m_latestRgba.resize(static_cast<size_t>(m_outputWidth) * m_outputHeight * 4);
        m_running = true;
        m_frameCount = 0;
        m_lastLogTime = std::chrono::steady_clock::now();
        XR_TUT_LOG("Camera2: Capture started.");
        return true;
    }

    void Stop() {
        m_running = false;

        if (m_imageReader) {
            AImageReader_setImageListener(m_imageReader, nullptr);
        }

        if (m_captureSession) {
            ACameraCaptureSession_stopRepeating(m_captureSession);
            ACameraCaptureSession_close(m_captureSession);
            m_captureSession = nullptr;
        }

        if (m_captureRequest) {
            ACaptureRequest_free(m_captureRequest);
            m_captureRequest = nullptr;
        }

        if (m_outputContainer) {
            ACaptureSessionOutputContainer_free(m_outputContainer);
            m_outputContainer = nullptr;
        }

        if (m_outputTarget) {
            ACameraOutputTarget_free(m_outputTarget);
            m_outputTarget = nullptr;
        }

        if (m_sessionOutput) {
            ACaptureSessionOutput_free(m_sessionOutput);
            m_sessionOutput = nullptr;
        }

        if (m_cameraDevice) {
            ACameraDevice_close(m_cameraDevice);
            m_cameraDevice = nullptr;
        }

        if (m_imageReader) {
            AImageReader_delete(m_imageReader);
            m_imageReader = nullptr;
        }

        if (m_cameraManager) {
            ACameraManager_delete(m_cameraManager);
            m_cameraManager = nullptr;
        }

    }

    bool IsRunning() const {
        return m_running;
    }

    bool GetLatestFrame(std::vector<uint8_t> &outRgba, uint32_t &width, uint32_t &height) {
        if (!m_running || !m_hasFrame) {
            return false;
        }

        std::lock_guard<std::mutex> lock(m_frameMutex);
        if (!m_hasFrame) {
            return false;
        }
        outRgba = m_latestRgba;
        width = m_outputWidth;
        height = m_outputHeight;
        m_hasFrame = false;
        return true;
    }

private:
    std::string SelectCameraId(const ACameraIdList *cameraIdList) {
        std::string bestId;
        int bestScore = -1;

        XR_TUT_LOG("Camera2: Found " << cameraIdList->numCameras << " cameras.");

        for (int i = 0; i < cameraIdList->numCameras; ++i) {
            const char *cameraId = cameraIdList->cameraIds[i];
            ACameraMetadata *metadata = nullptr;
            if (ACameraManager_getCameraCharacteristics(m_cameraManager, cameraId, &metadata) != ACAMERA_OK || !metadata) {
                continue;
            }

            // Log all metadata tags for this camera
            int32_t numEntries = 0;
            const uint32_t *tags = nullptr;
            ACameraMetadata_getAllTags(metadata, &numEntries, &tags);
            XR_TUT_LOG("Camera2: Camera " << cameraId << " has " << numEntries << " metadata tags.");

            ACameraMetadata_const_entry entry{};
            int lensScore = 0;
            if (ACameraMetadata_getConstEntry(metadata, ACAMERA_LENS_FACING, &entry) == ACAMERA_OK && entry.count > 0) {
                if (entry.data.u8[0] == ACAMERA_LENS_FACING_EXTERNAL) {
                    lensScore = 2;
                    XR_TUT_LOG("Camera2: Camera " << cameraId << " lens facing EXTERNAL (passthrough candidate).");
                } else if (entry.data.u8[0] == ACAMERA_LENS_FACING_BACK) {
                    lensScore = 1;
                    XR_TUT_LOG("Camera2: Camera " << cameraId << " lens facing BACK.");
                } else if (entry.data.u8[0] == ACAMERA_LENS_FACING_FRONT) {
                    XR_TUT_LOG("Camera2: Camera " << cameraId << " lens facing FRONT.");
                } else {
                    XR_TUT_LOG("Camera2: Camera " << cameraId << " lens facing UNKNOWN (" << (int)entry.data.u8[0] << ").");
                }
            }

            if (lensScore > bestScore) {
                bestScore = lensScore;
                bestId = cameraId;
            }

            ACameraMetadata_free(metadata);
        }

        if (bestId.empty() && cameraIdList->numCameras > 0) {
            // Fallback: just use the first camera
            bestId = cameraIdList->cameraIds[0];
            XR_TUT_LOG("Camera2: No external camera found, falling back to camera " << bestId);
        }

        return bestId;
    }

    bool SelectCameraSize(const char *cameraId, uint32_t desiredWidth, uint32_t desiredHeight) {
        ACameraMetadata *metadata = nullptr;
        if (ACameraManager_getCameraCharacteristics(m_cameraManager, cameraId, &metadata) != ACAMERA_OK || !metadata) {
            return false;
        }

        ACameraMetadata_const_entry entry{};
        if (ACameraMetadata_getConstEntry(metadata, ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS, &entry) != ACAMERA_OK) {
            ACameraMetadata_free(metadata);
            return false;
        }

        int bestScore = INT32_MAX;
        for (uint32_t i = 0; i + 3 < entry.count; i += 4) {
            int32_t format = entry.data.i32[i + 0];
            int32_t width = entry.data.i32[i + 1];
            int32_t height = entry.data.i32[i + 2];
            int32_t output = entry.data.i32[i + 3];

            if (format != AIMAGE_FORMAT_YUV_420_888 || output != ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS_OUTPUT) {
                continue;
            }

            int score = std::abs(width - static_cast<int32_t>(desiredWidth)) + std::abs(height - static_cast<int32_t>(desiredHeight));
            if (score < bestScore) {
                bestScore = score;
                m_cameraWidth = static_cast<uint32_t>(width);
                m_cameraHeight = static_cast<uint32_t>(height);
            }
        }

        ACameraMetadata_free(metadata);
        if (m_cameraWidth == 0 || m_cameraHeight == 0) {
            return false;
        }
        XR_TUT_LOG("Camera2: Selected size " << m_cameraWidth << "x" << m_cameraHeight);
        return true;
    }

    static void OnImageAvailable(void *context, AImageReader *reader) {
        auto *self = reinterpret_cast<Camera2Capture *>(context);
        if (!self) {
            return;
        }
        if (!self->m_running) {
            return;
        }

        AImage *image = nullptr;
        if (AImageReader_acquireLatestImage(reader, &image) != AMEDIA_OK || !image) {
            return;
        }

        int32_t imageFormat = 0;
        if (AImage_getFormat(image, &imageFormat) != AMEDIA_OK || imageFormat != AIMAGE_FORMAT_YUV_420_888) {
            AImage_delete(image);
            return;
        }

        int32_t srcWidth = 0;
        int32_t srcHeight = 0;
        AImage_getWidth(image, &srcWidth);
        AImage_getHeight(image, &srcHeight);

        uint8_t *planeData[3] = {nullptr, nullptr, nullptr};
        int planeLength[3] = {0, 0, 0};
        int rowStride[3] = {0, 0, 0};
        int pixelStride[3] = {0, 0, 0};

        for (int i = 0; i < 3; ++i) {
            if (AImage_getPlaneData(image, i, &planeData[i], &planeLength[i]) != AMEDIA_OK) {
                AImage_delete(image);
                return;
            }
            if (AImage_getPlaneRowStride(image, i, &rowStride[i]) != AMEDIA_OK) {
                AImage_delete(image);
                return;
            }
            if (AImage_getPlanePixelStride(image, i, &pixelStride[i]) != AMEDIA_OK) {
                AImage_delete(image);
                return;
            }
        }

        if (!planeData[0] || !planeData[1] || !planeData[2]) {
            AImage_delete(image);
            return;
        }

        if (rowStride[0] <= 0 || rowStride[1] <= 0 || rowStride[2] <= 0) {
            AImage_delete(image);
            return;
        }
        if (pixelStride[0] == 0 || pixelStride[1] == 0 || pixelStride[2] == 0) {
            AImage_delete(image);
            return;
        }

        const uint32_t outputWidth = self->m_outputWidth;
        const uint32_t outputHeight = self->m_outputHeight;
        if (outputWidth == 0 || outputHeight == 0) {
            AImage_delete(image);
            return;
        }

        std::lock_guard<std::mutex> lock(self->m_frameMutex);
        if (self->m_latestRgba.size() != static_cast<size_t>(outputWidth) * outputHeight * 4) {
            self->m_latestRgba.resize(static_cast<size_t>(outputWidth) * outputHeight * 4);
        }

        const bool interleavedUV = (planeData[1] == planeData[2]);
        if (interleavedUV) {
            self->m_uvIsVU = false;
        }

        if (!self->m_loggedFormat) {
            XR_TUT_LOG("Camera2: Y plane rowStride=" << rowStride[0] << " pixelStride=" << pixelStride[0] << " len=" << planeLength[0]);
            XR_TUT_LOG("Camera2: U plane rowStride=" << rowStride[1] << " pixelStride=" << pixelStride[1] << " len=" << planeLength[1]);
            XR_TUT_LOG("Camera2: V plane rowStride=" << rowStride[2] << " pixelStride=" << pixelStride[2] << " len=" << planeLength[2]);
            XR_TUT_LOG("Camera2: interleavedUV=" << (interleavedUV ? "true" : "false") << " uvIsVU=" << (self->m_uvIsVU ? "true" : "false"));
            // Log first few Y values to verify we're getting real data
            XR_TUT_LOG("Camera2: First 8 Y values: "
                << (int)planeData[0][0] << " " << (int)planeData[0][1] << " "
                << (int)planeData[0][2] << " " << (int)planeData[0][3] << " "
                << (int)planeData[0][4] << " " << (int)planeData[0][5] << " "
                << (int)planeData[0][6] << " " << (int)planeData[0][7]);
            // Log first few U/V values
            XR_TUT_LOG("Camera2: First 4 U values: "
                << (int)planeData[1][0] << " " << (int)planeData[1][2] << " "
                << (int)planeData[1][4] << " " << (int)planeData[1][6]);
            XR_TUT_LOG("Camera2: First 4 V values: "
                << (int)planeData[2][0] << " " << (int)planeData[2][2] << " "
                << (int)planeData[2][4] << " " << (int)planeData[2][6]);
            self->m_loggedFormat = true;
        }

        for (uint32_t y = 0; y < outputHeight; ++y) {
            uint32_t srcY = static_cast<uint32_t>(y * srcHeight / outputHeight);
            const uint8_t *yRow = planeData[0] + srcY * rowStride[0];
            const uint8_t *uRow = planeData[1] + (srcY / 2) * rowStride[1];
            const uint8_t *vRow = planeData[2] + (srcY / 2) * rowStride[2];

            for (uint32_t x = 0; x < outputWidth; ++x) {
                uint32_t srcX = static_cast<uint32_t>(x * srcWidth / outputWidth);
                int yIndex = static_cast<int>(srcX * pixelStride[0]);
                int uIndex = static_cast<int>((srcX / 2) * pixelStride[1]);
                int vIndex = static_cast<int>((srcX / 2) * pixelStride[2]);

                size_t yOffset = static_cast<size_t>(srcY) * static_cast<size_t>(rowStride[0]);
                size_t uOffset = static_cast<size_t>(srcY / 2) * static_cast<size_t>(rowStride[1]);
                size_t vOffset = static_cast<size_t>(srcY / 2) * static_cast<size_t>(rowStride[2]);

                if (yIndex < 0 || uIndex < 0 || vIndex < 0) {
                    continue;
                }
                if (yIndex >= rowStride[0] || uIndex >= rowStride[1] || vIndex >= rowStride[2]) {
                    continue;
                }
                if (yOffset + static_cast<size_t>(yIndex) >= static_cast<size_t>(planeLength[0])) {
                    continue;
                }

                uint8_t yValue = yRow[yIndex];
                uint8_t uValue = 128;
                uint8_t vValue = 128;

                if (interleavedUV) {
                    size_t uvOffset = static_cast<size_t>(srcY / 2) * static_cast<size_t>(rowStride[1]);
                    size_t uvIndex = static_cast<size_t>(uIndex);
                    if (uvOffset + uvIndex + 1 < static_cast<size_t>(planeLength[1])) {
                        if (self->m_uvIsVU) {
                            vValue = uRow[uvIndex];
                            uValue = uRow[uvIndex + 1];
                        } else {
                            uValue = uRow[uvIndex];
                            vValue = uRow[uvIndex + 1];
                        }
                    }
                } else {
                    if (uOffset + static_cast<size_t>(uIndex) < static_cast<size_t>(planeLength[1])) {
                        uValue = uRow[uIndex];
                    }
                    if (vOffset + static_cast<size_t>(vIndex) < static_cast<size_t>(planeLength[2])) {
                        vValue = vRow[vIndex];
                    }
                }

                int yLum = static_cast<int>(yValue);
                int u = static_cast<int>(uValue) - 128;
                int v = static_cast<int>(vValue) - 128;

                int r = static_cast<int>(yLum + 1.402f * v);
                int g = static_cast<int>(yLum - 0.344136f * u - 0.714136f * v);
                int b = static_cast<int>(yLum + 1.772f * u);

                size_t outIndex = (static_cast<size_t>(y) * outputWidth + x) * 4;
                self->m_latestRgba[outIndex + 0] = ClampToByte(r);
                self->m_latestRgba[outIndex + 1] = ClampToByte(g);
                self->m_latestRgba[outIndex + 2] = ClampToByte(b);
                self->m_latestRgba[outIndex + 3] = 255;
            }
        }

        self->m_hasFrame = true;
        self->m_frameCount++;
        auto now = std::chrono::steady_clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - self->m_lastLogTime).count();
        if (elapsedMs >= 2000) {
            double seconds = elapsedMs / 1000.0;
            double fps = static_cast<double>(self->m_frameCount) / seconds;
            // Log some RGBA values from the center of the converted image
            size_t centerY = outputHeight / 2;
            size_t centerX = outputWidth / 2;
            size_t centerIdx = (centerY * outputWidth + centerX) * 4;
            if (centerIdx + 3 < self->m_latestRgba.size()) {
                XR_TUT_LOG("Camera2: FPS=" << fps << " Center RGBA=("
                    << (int)self->m_latestRgba[centerIdx] << ","
                    << (int)self->m_latestRgba[centerIdx+1] << ","
                    << (int)self->m_latestRgba[centerIdx+2] << ","
                    << (int)self->m_latestRgba[centerIdx+3] << ")");
            } else {
                XR_TUT_LOG("Camera2: FPS=" << fps);
            }
            self->m_frameCount = 0;
            self->m_lastLogTime = now;
        }
        AImage_delete(image);
    }

    static void OnCameraDisconnected(void *context, ACameraDevice *device) {
        (void)context;
        (void)device;
        XR_TUT_LOG("Camera2: Camera disconnected.");
    }

    static void OnCameraError(void *context, ACameraDevice *device, int error) {
        (void)context;
        (void)device;
        XR_TUT_LOG_ERROR("Camera2: Camera error " << error);
    }

    static void OnSessionClosed(void *context, ACameraCaptureSession *session) {
        (void)context;
        (void)session;
        XR_TUT_LOG("Camera2: Session closed.");
    }

    static void OnSessionReady(void *context, ACameraCaptureSession *session) {
        (void)context;
        (void)session;
        XR_TUT_LOG("Camera2: Session ready.");
    }

    static void OnSessionActive(void *context, ACameraCaptureSession *session) {
        (void)context;
        (void)session;
        XR_TUT_LOG("Camera2: Session active.");
    }

private:
    ACameraManager *m_cameraManager = nullptr;
    ACameraDevice *m_cameraDevice = nullptr;
    ACameraCaptureSession *m_captureSession = nullptr;
    ACaptureRequest *m_captureRequest = nullptr;
    ACameraOutputTarget *m_outputTarget = nullptr;
    ACaptureSessionOutputContainer *m_outputContainer = nullptr;
    ACaptureSessionOutput *m_sessionOutput = nullptr;
    AImageReader *m_imageReader = nullptr;
    ANativeWindow *m_imageReaderWindow = nullptr;

    ACameraDevice_stateCallbacks m_deviceCallbacks{};
    ACameraCaptureSession_stateCallbacks m_sessionCallbacks{};
    AImageReader_ImageListener m_imageListener{};

    std::string m_cameraId;
    uint32_t m_cameraWidth = 0;
    uint32_t m_cameraHeight = 0;
    uint32_t m_outputWidth = 0;
    uint32_t m_outputHeight = 0;

    std::mutex m_frameMutex;
    std::vector<uint8_t> m_latestRgba;
    bool m_hasFrame = false;
    std::atomic<bool> m_running = false;
    uint64_t m_frameCount = 0;
    std::chrono::steady_clock::time_point m_lastLogTime{};
    bool m_uvIsVU = false;
    bool m_loggedFormat = false;
};

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
        UpdateRenderSize();

        while (m_applicationRunning) {
            PollSystemEvents();
            PollEvents();
            if (m_sessionRunning) {
                MaybeStartCamera();
                RenderFrame();
            }
        }

        m_camera.Stop();
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
        // Create LOCAL space (for potential future use)
        XrReferenceSpaceCreateInfo referenceSpaceCI{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
        referenceSpaceCI.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
        referenceSpaceCI.poseInReferenceSpace = {{0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
        OPENXR_CHECK(xrCreateReferenceSpace(m_session, &referenceSpaceCI, &m_localSpace), "Failed to create Local ReferenceSpace.");

        // Create VIEW space (head-locked) for the camera passthrough quad
        referenceSpaceCI.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_VIEW;
        referenceSpaceCI.poseInReferenceSpace = {{0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
        OPENXR_CHECK(xrCreateReferenceSpace(m_session, &referenceSpaceCI, &m_viewSpace), "Failed to create View ReferenceSpace.");
    }

    void DestroyReferenceSpace() {
        if (m_viewSpace != XR_NULL_HANDLE) {
            OPENXR_CHECK(xrDestroySpace(m_viewSpace), "Failed to destroy View ReferenceSpace.");
            m_viewSpace = XR_NULL_HANDLE;
        }
        if (m_localSpace != XR_NULL_HANDLE) {
            OPENXR_CHECK(xrDestroySpace(m_localSpace), "Failed to destroy Local ReferenceSpace.");
            m_localSpace = XR_NULL_HANDLE;
        }
    }

    void CreateSwapchains() {
        uint32_t formatCount = 0;
        OPENXR_CHECK(xrEnumerateSwapchainFormats(m_session, 0, &formatCount, nullptr), "Failed to enumerate Swapchain Formats.");
        std::vector<int64_t> formats(formatCount);
        OPENXR_CHECK(xrEnumerateSwapchainFormats(m_session, formatCount, &formatCount, formats.data()), "Failed to enumerate Swapchain Formats.");

        const int64_t colorFormat = m_graphicsAPI->SelectColorSwapchainFormat(formats);

        // Create a single swapchain for the quad layer (not per-view)
        // Use the first view's recommended size as the quad texture size
        const XrViewConfigurationView &view = m_viewConfigurationViews[0];

        XrSwapchainCreateInfo swapchainCI{XR_TYPE_SWAPCHAIN_CREATE_INFO};
        swapchainCI.usageFlags = XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT | XR_SWAPCHAIN_USAGE_TRANSFER_DST_BIT;
        swapchainCI.format = colorFormat;
        swapchainCI.sampleCount = view.recommendedSwapchainSampleCount;
        swapchainCI.width = view.recommendedImageRectWidth;
        swapchainCI.height = view.recommendedImageRectHeight;
        swapchainCI.faceCount = 1;
        swapchainCI.arraySize = 1;
        swapchainCI.mipCount = 1;

        OPENXR_CHECK(xrCreateSwapchain(m_session, &swapchainCI, &m_quadSwapchain.swapchain), "Failed to create Quad Swapchain.");
        m_quadSwapchain.swapchainFormat = colorFormat;
        m_quadSwapchain.width = swapchainCI.width;
        m_quadSwapchain.height = swapchainCI.height;
        XR_TUT_LOG("Quad Swapchain created: " << m_quadSwapchain.width << "x" << m_quadSwapchain.height << " format=" << colorFormat);

        uint32_t imageCount = 0;
        OPENXR_CHECK(xrEnumerateSwapchainImages(m_quadSwapchain.swapchain, 0, &imageCount, nullptr), "Failed to enumerate Swapchain Images.");
        XrSwapchainImageBaseHeader *images = m_graphicsAPI->AllocateSwapchainImageData(m_quadSwapchain.swapchain, GraphicsAPI::SwapchainType::COLOR, imageCount);
        OPENXR_CHECK(xrEnumerateSwapchainImages(m_quadSwapchain.swapchain, imageCount, &imageCount, images), "Failed to enumerate Swapchain Images.");

        m_quadSwapchain.imageViews.resize(imageCount);
        for (uint32_t imageIndex = 0; imageIndex < imageCount; ++imageIndex) {
            void *image = m_graphicsAPI->GetSwapchainImage(m_quadSwapchain.swapchain, imageIndex);
            GraphicsAPI::ImageViewCreateInfo imageViewCI{};
            imageViewCI.image = image;
            imageViewCI.type = GraphicsAPI::ImageViewCreateInfo::Type::RTV;
            imageViewCI.view = GraphicsAPI::ImageViewCreateInfo::View::TYPE_2D;
            imageViewCI.format = m_quadSwapchain.swapchainFormat;
            imageViewCI.aspect = GraphicsAPI::ImageViewCreateInfo::Aspect::COLOR_BIT;
            imageViewCI.baseMipLevel = 0;
            imageViewCI.levelCount = 1;
            imageViewCI.baseArrayLayer = 0;
            imageViewCI.layerCount = 1;
            m_quadSwapchain.imageViews[imageIndex] = m_graphicsAPI->CreateImageView(imageViewCI);
        }

        // Calculate quad size to fill field of view at the given distance
        // Using ~100 degree horizontal FOV coverage at m_quadDistance meters
        const float horizontalFovRad = 1.75f;  // ~100 degrees in radians
        m_quadWidth = 2.0f * m_quadDistance * std::tan(horizontalFovRad / 2.0f);
        // Maintain aspect ratio of the swapchain
        float aspectRatio = static_cast<float>(m_quadSwapchain.width) / static_cast<float>(m_quadSwapchain.height);
        m_quadHeight = m_quadWidth / aspectRatio;
        XR_TUT_LOG("Quad size: " << m_quadWidth << "m x " << m_quadHeight << "m at distance " << m_quadDistance << "m");
    }

    void UpdateRenderSize() {
        if (!m_viewConfigurationViews.empty()) {
            m_renderWidth = m_viewConfigurationViews[0].recommendedImageRectWidth;
            m_renderHeight = m_viewConfigurationViews[0].recommendedImageRectHeight;
        }
    }

    void MaybeStartCamera() {
        if (m_camera.IsRunning()) {
            return;
        }
        if (!g_cameraPermissionGranted.load()) {
            XR_TUT_LOG("Camera2: Waiting for camera permission.");
            return;
        }
        if (m_renderWidth == 0 || m_renderHeight == 0) {
            return;
        }
        m_camera.Start(m_renderWidth, m_renderHeight);
    }

    void DestroySwapchains() {
        // Destroy quad swapchain
        if (m_quadSwapchain.swapchain != XR_NULL_HANDLE) {
            for (void *&imageView : m_quadSwapchain.imageViews) {
                m_graphicsAPI->DestroyImageView(imageView);
            }
            m_graphicsAPI->FreeSwapchainImageData(m_quadSwapchain.swapchain);
            OPENXR_CHECK(xrDestroySwapchain(m_quadSwapchain.swapchain), "Failed to destroy Quad Swapchain.");
            m_quadSwapchain = {};
        }
    }

    struct RenderLayerInfo {
        XrTime predictedDisplayTime = 0;
        std::vector<XrCompositionLayerBaseHeader *> layers;
        XrCompositionLayerQuad layerQuad = {XR_TYPE_COMPOSITION_LAYER_QUAD};
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
                renderLayerInfo.layers.push_back(reinterpret_cast<XrCompositionLayerBaseHeader *>(&renderLayerInfo.layerQuad));
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
        // Acquire the quad swapchain image
        uint32_t imageIndex = 0;
        XrSwapchainImageAcquireInfo acquireInfo{XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO};
        OPENXR_CHECK(xrAcquireSwapchainImage(m_quadSwapchain.swapchain, &acquireInfo, &imageIndex), "Failed to acquire Quad Swapchain Image.");

        XrSwapchainImageWaitInfo waitInfo{XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO};
        waitInfo.timeout = XR_INFINITE_DURATION;
        OPENXR_CHECK(xrWaitSwapchainImage(m_quadSwapchain.swapchain, &waitInfo), "Failed to wait for Quad Swapchain Image.");

        // Get latest camera frame
        std::vector<uint8_t> frame;
        uint32_t frameWidth = 0;
        uint32_t frameHeight = 0;
        if (m_camera.GetLatestFrame(frame, frameWidth, frameHeight)) {
            m_cachedFrame = std::move(frame);
            m_cachedWidth = frameWidth;
            m_cachedHeight = frameHeight;
            m_hasCachedFrame = true;
            if (IsBgraFormat(m_quadSwapchain.swapchainFormat)) {
                SwizzleRgbaToBgra(m_cachedFrame);
                m_cachedIsBgra = true;
            } else {
                m_cachedIsBgra = false;
            }
        }

        // Render camera frame to quad swapchain
        if (m_hasCachedFrame) {
            auto *graphicsApiVulkan = static_cast<GraphicsAPI_Vulkan *>(m_graphicsAPI.get());
            graphicsApiVulkan->UploadRgbaToImageCentered(
                m_quadSwapchain.imageViews[imageIndex],
                m_quadSwapchain.width, m_quadSwapchain.height,
                m_cachedFrame.data(),
                m_cachedWidth, m_cachedHeight);
            m_uploadCount++;
            if (m_uploadCount <= 3 || m_uploadCount % 100 == 0) {
                XR_TUT_LOG("Vulkan: Upload #" << m_uploadCount
                    << " src=" << m_cachedWidth << "x" << m_cachedHeight
                    << " dst=" << m_quadSwapchain.width << "x" << m_quadSwapchain.height
                    << " format=" << m_quadSwapchain.swapchainFormat);
            }
        } else {
            m_graphicsAPI->BeginRendering();
            m_graphicsAPI->ClearColor(m_quadSwapchain.imageViews[imageIndex], 0.0f, 0.0f, 0.0f, 1.0f);
            m_graphicsAPI->EndRendering();
            m_clearCount++;
            if (m_clearCount <= 5) {
                XR_TUT_LOG("Vulkan: Clear #" << m_clearCount << " (no camera frame yet)");
            }
        }

        XrSwapchainImageReleaseInfo releaseInfo{XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO};
        OPENXR_CHECK(xrReleaseSwapchainImage(m_quadSwapchain.swapchain, &releaseInfo), "Failed to release Quad Swapchain Image.");

        // Configure quad layer
        // Position the quad directly in front of the user, facing them
        // Using VIEW space makes it head-locked (follows head movement)
        renderLayerInfo.layerQuad.type = XR_TYPE_COMPOSITION_LAYER_QUAD;
        renderLayerInfo.layerQuad.layerFlags = XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT;
        renderLayerInfo.layerQuad.space = m_viewSpace;  // Head-locked
        renderLayerInfo.layerQuad.eyeVisibility = XR_EYE_VISIBILITY_BOTH;

        // Subimage references the entire swapchain texture
        renderLayerInfo.layerQuad.subImage.swapchain = m_quadSwapchain.swapchain;
        renderLayerInfo.layerQuad.subImage.imageRect.offset = {0, 0};
        renderLayerInfo.layerQuad.subImage.imageRect.extent.width = static_cast<int32_t>(m_quadSwapchain.width);
        renderLayerInfo.layerQuad.subImage.imageRect.extent.height = static_cast<int32_t>(m_quadSwapchain.height);
        renderLayerInfo.layerQuad.subImage.imageArrayIndex = 0;

        // Position quad in front of user (in local space, looking down -Z)
        // Pose: position at (0, 0, -distance), orientation facing +Z (toward user)
        renderLayerInfo.layerQuad.pose.position = {0.0f, 0.0f, -m_quadDistance};
        renderLayerInfo.layerQuad.pose.orientation = {0.0f, 0.0f, 0.0f, 1.0f};  // Identity = facing +Z

        // Size of quad in meters
        renderLayerInfo.layerQuad.size = {m_quadWidth, m_quadHeight};

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
        uint32_t width = 0;
        uint32_t height = 0;
        std::vector<void *> imageViews;
    };
    SwapchainInfo m_quadSwapchain = {};  // Single swapchain for quad layer

    // Quad layer positioning (adjustable for zoom functionality)
    float m_quadDistance = 10.0f;   // Distance from user in meters (far = feels like infinity)
    float m_quadWidth = 0.0f;       // Calculated in CreateSwapchains based on FOV
    float m_quadHeight = 0.0f;      // Calculated to maintain aspect ratio

    std::vector<XrEnvironmentBlendMode> m_applicationEnvironmentBlendModes = {XR_ENVIRONMENT_BLEND_MODE_OPAQUE};
    std::vector<XrEnvironmentBlendMode> m_environmentBlendModes = {};
    XrEnvironmentBlendMode m_environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_MAX_ENUM;

    XrSpace m_localSpace = XR_NULL_HANDLE;
    XrSpace m_viewSpace = XR_NULL_HANDLE;   // Head-locked reference space for quad layer

    uint32_t m_renderWidth = 0;
    uint32_t m_renderHeight = 0;

    Camera2Capture m_camera;
    std::vector<uint8_t> m_cachedFrame;
    uint32_t m_cachedWidth = 0;
    uint32_t m_cachedHeight = 0;
    bool m_hasCachedFrame = false;
    bool m_cachedIsBgra = false;
    uint64_t m_uploadCount = 0;
    uint64_t m_clearCount = 0;
};

void Camera2Tutorial_Main() {
    DebugOutput debugOutput;
    XR_TUT_LOG("OpenXR Camera2 Tutorial");

    Camera2Tutorial app;
    app.Run();
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_openxrcamera2tutorial_Camera2TutorialActivity_nativeOnPermissionResult(JNIEnv *env, jclass clazz, jboolean granted) {
    (void)env;
    (void)clazz;
    g_cameraPermissionGranted.store(granted == JNI_TRUE);
    XR_TUT_LOG("Camera2: Permission " << (granted == JNI_TRUE ? "granted" : "denied"));
}

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
