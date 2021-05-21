#pragma once

#include "Core.h"
#include "JuniperContext.h"

#include <memory>
#include <vector>

namespace jun {
    class JuniperCommandBuffer;
}

class jun::JuniperCommandBuffer {
    public:
        JuniperCommandBuffer(const JuniperContext& context);
        ~JuniperCommandBuffer() = default;

        void cleanup();
    private:
        std::shared_ptr<VkSurfaceKHR> mpSurface;
        std::shared_ptr<VkPhysicalDevice> mpPhysicalDevice;
        std::shared_ptr<VkDevice> mpDevice;
        std::shared_ptr<VkCommandPool> mpCommandPool;

        void createCommandPool();
        void createCommandBuffers();
};