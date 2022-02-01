#pragma once

#include <memory>
#include <vector>

#include "Core.h"
#include "JuniperContext.h"

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
        std::shared_ptr<VkExtent2D> mpSwapChainExtent;
        std::shared_ptr<VkRenderPass> mpRenderPass;
        std::shared_ptr<VkPipeline> mpGraphicsPipeline;
        std::shared_ptr<std::vector<VkFramebuffer>> mpSwapChainFramebuffers;
        std::shared_ptr<VkCommandPool> mpCommandPool;
        std::shared_ptr<std::vector<VkCommandBuffer>> mpCommandBuffers;

        void createCommandPool();
        void createCommandBuffers();
};