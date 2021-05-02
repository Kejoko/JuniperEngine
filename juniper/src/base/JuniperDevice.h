#pragma once

#include "Core.h"

#include <memory>
#include <optional>

namespace jun {
    class JuniperDevice;
}

class jun::JuniperDevice {
    public:
        JuniperDevice(std::shared_ptr<VkInstance> pInstance);
        ~JuniperDevice() = default;

        void cleanup();

    private:
        std::shared_ptr<VkInstance> mpInstance;
        VkPhysicalDevice mPhysicalDevice;
        VkDevice mDevice;
        VkQueue mGraphicsQueue;

        const std::vector<const char*> mValidationLayers = {"VK_LAYER_KHRONOS_validation"};
        #if defined(BUILD_DEBUG)
        const bool mEnableValidationLayers = true;
        #else
        const bool mEnableValidationLayers = false;
        #endif

        struct QueueFamilyIndices;

        void pickPhysicalDevice();
        int rateDeviceSuitability(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        void createLogicalDevice();
};

struct jun::JuniperDevice::QueueFamilyIndices  {
    std::optional<uint32_t> mGraphicsFamily;

    bool isComplete();
};