#pragma once

#include "Core.h"
#include "AppInfo.h"

#include <memory>
#include <optional>

namespace jun {
    class JuniperDevice;
    struct QueueFamilyIndices;
    struct SwapChainSupportDetails;
}

class jun::JuniperDevice {
    public:
        // JuniperDevice(const AppInfo& info, std::shared_ptr<VkInstance> pInstance, std::shared_ptr<GLFWwindow*> ppWindow, std::shared_ptr<VkSurfaceKHR> pSurface);
        JuniperDevice(const AppInfo& info,
                      std::shared_ptr<VkInstance> pInstance,
                      std::shared_ptr<GLFWwindow*> ppWindow,
                      std::shared_ptr<VkSurfaceKHR> pSurface,
                      std::shared_ptr<VkPhysicalDevice> pPhysicalDevice,
                      std::shared_ptr<VkDevice> pDevice,
                      std::shared_ptr<VkQueue> pGraphicsQueue,
                      std::shared_ptr<VkQueue> pPresentQueue);
        ~JuniperDevice() = default;

        void cleanup();

    private:
        const std::vector<const char*> mValidationLayers;
        const std::vector<const char*> mDeviceExtensions;
        const bool mEnableValidationLayers;
        std::shared_ptr<VkInstance> mpInstance;
        std::shared_ptr<GLFWwindow*> mppWindow;
        std::shared_ptr<VkSurfaceKHR> mpSurface;
        // VkPhysicalDevice mPhysicalDevice;
        // VkDevice mDevice;
        // VkQueue mGraphicsQueue;
        // VkQueue mPresentQueue;
        std::shared_ptr<VkPhysicalDevice> mpPhysicalDevice;
        std::shared_ptr<VkDevice> mpDevice;
        std::shared_ptr<VkQueue> mpGraphicsQueue;
        std::shared_ptr<VkQueue> mpPresentQueue;

        void pickPhysicalDevice();
        int rateDeviceSuitability(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        void createLogicalDevice();
};

struct jun::QueueFamilyIndices {
    std::optional<uint32_t> mGraphicsFamily;
    std::optional<uint32_t> mPresentFamily;

    void findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    bool isComplete();
};

struct jun::SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR mCapabilities;
    std::vector<VkSurfaceFormatKHR> mFormats;
    std::vector<VkPresentModeKHR> mPresentModes;

    void querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR pSurface);
};