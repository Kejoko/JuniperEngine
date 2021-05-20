#pragma once

#include "Core.h"
#include "JuniperContext.h"

#include <memory>
#include <vector>

namespace jun {
    class JuniperFrameBuffer;
}

class jun::JuniperFrameBuffer{
    public:
        JuniperFrameBuffer(const JuniperContext& context);
        ~JuniperFrameBuffer() = default;

        void cleanup();
    private:
        std::shared_ptr<VkDevice> mpDevice;
        std::shared_ptr<VkExtent2D> mpSwapChainExtent;
        std::shared_ptr<std::vector<VkImageView>> mpSwapChainImageViews;
        std::shared_ptr<VkRenderPass> mpRenderPass;
        std::shared_ptr<std::vector<VkFramebuffer>> mpSwapChainFramebuffers;

        void createFrameBuffers();
};