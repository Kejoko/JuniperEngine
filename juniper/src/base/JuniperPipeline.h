#pragma once

#include <memory>
#include <vector>

#include "Core.h"
#include "JuniperContext.h"

namespace jun {
    class JuniperPipeline;
}

class jun::JuniperPipeline {
    public:
        JuniperPipeline(const JuniperContext& context);
        ~JuniperPipeline() = default;

        void cleanup();
    private:
        std::shared_ptr<VkDevice> mpDevice;
        std::shared_ptr<VkFormat> mpSwapChainImageFormat;
        std::shared_ptr<VkExtent2D> mpSwapChainExtent;
        std::shared_ptr<VkRenderPass> mpRenderPass;
        std::shared_ptr<VkPipelineLayout> mpPipelineLayout;
        std::shared_ptr<VkPipeline> mpGraphicsPipeline;

        void createRenderPass();
        void createGraphicsPipeline();
        VkShaderModule createShaderModule(const std::vector<char>& byteCode);
};