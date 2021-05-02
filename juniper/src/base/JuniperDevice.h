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

        struct QueueFamilyIndices;

        void pickPhysicalDevice();
        int rateDeviceSuitability(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};

struct jun::JuniperDevice::QueueFamilyIndices  {
    std::optional<uint32_t> mGraphicsFamily;

    bool isComplete();
};