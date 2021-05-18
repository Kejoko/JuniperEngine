#pragma once

#include "Core.h"
#include "JuniperContext.h"

#include <memory>
#include <vector>

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
        std::shared_ptr<VkExtent2D> mpSwapChainExtent;
        std::shared_ptr<VkPipelineLayout> mpPipelineLayout;

        void createGraphicsPipeline();
        VkShaderModule createShaderModule(const std::vector<char>& byteCode);
};