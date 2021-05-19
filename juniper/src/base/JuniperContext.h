#pragma once

#include "Core.h"

#include <memory>
#include <vector>

namespace jun {
    class JuniperContext;
}

class jun::JuniperContext {
    public:
        JuniperContext() = default;
        ~JuniperContext() = default;

        std::shared_ptr<VkInstance> mpInstance = std::make_shared<VkInstance>();
        std::shared_ptr<GLFWwindow*> mppWindow = std::make_shared<GLFWwindow*>();
        std::shared_ptr<VkSurfaceKHR> mpSurface = std::make_shared<VkSurfaceKHR>();
        std::shared_ptr<VkPhysicalDevice> mpPhysicalDevice = std::make_shared<VkPhysicalDevice>();
        std::shared_ptr<VkDevice> mpDevice = std::make_shared<VkDevice>();
        std::shared_ptr<VkQueue> mpGraphicsQueue = std::make_shared<VkQueue>();
        std::shared_ptr<VkQueue> mpPresentQueue = std::make_shared<VkQueue>();
        std::shared_ptr<VkSwapchainKHR> mpSwapChain = std::make_shared<VkSwapchainKHR>();
        std::shared_ptr<std::vector<VkImage>> mpSwapChainImages = std::make_shared<std::vector<VkImage>>();
        std::shared_ptr<VkFormat> mpSwapChainImageFormat = std::make_shared<VkFormat>();
        std::shared_ptr<VkExtent2D> mpSwapChainExtent = std::make_shared<VkExtent2D>();
        std::shared_ptr<std::vector<VkImageView>> mpSwapChainImageViews = std::make_shared<std::vector<VkImageView>>();
        std::shared_ptr<VkRenderPass> mpRenderPass = std::make_shared<VkRenderPass>();
        std::shared_ptr<VkPipelineLayout> mpPipelineLayout = std::make_shared<VkPipelineLayout>();
};