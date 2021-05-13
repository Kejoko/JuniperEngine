#pragma once

#include "Core.h"
#include "AppInfo.h"

#include <memory>
#include <optional>

namespace jun {
    class JuniperDevice;
}

class jun::JuniperDevice {
    public:
        JuniperDevice(const AppInfo& info, std::shared_ptr<VkInstance> pInstance, std::shared_ptr<GLFWwindow*> ppWindow, std::shared_ptr<VkSurfaceKHR> pSurface);
        ~JuniperDevice() = default;

        void cleanup();

    private:
        const std::vector<const char*> mValidationLayers;
        const std::vector<const char*> mDeviceExtensions;
        const bool mEnableValidationLayers;
        std::shared_ptr<VkInstance> mpInstance;
        std::shared_ptr<GLFWwindow*> mppWindow;
        std::shared_ptr<VkSurfaceKHR> mpSurface;
        VkPhysicalDevice mPhysicalDevice;
        VkDevice mDevice;
        VkQueue mGraphicsQueue;
        VkQueue mPresentQueue;

        struct QueueFamilyIndices;
        struct SwapChainSupportDetails;

        void pickPhysicalDevice();
        int rateDeviceSuitability(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void createLogicalDevice();
};

struct jun::JuniperDevice::QueueFamilyIndices {
    std::optional<uint32_t> mGraphicsFamily;
    std::optional<uint32_t> mPresentFamily;

    bool isComplete();
};

struct jun::JuniperDevice::SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR mCapabilities;
    std::vector<VkSurfaceFormatKHR> mFormats;
    std::vector<VkPresentModeKHR> mPresentModes;
};