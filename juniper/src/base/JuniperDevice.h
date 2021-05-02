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
        JuniperDevice(const AppInfo& info, std::shared_ptr<VkInstance> pInstance);
        ~JuniperDevice() = default;

        void cleanup();

    private:
        const std::vector<const char*> mValidationLayers;
        const bool mEnableValidationLayers;
        std::shared_ptr<VkInstance> mpInstance;
        VkPhysicalDevice mPhysicalDevice;
        VkDevice mDevice;
        VkQueue mGraphicsQueue;

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