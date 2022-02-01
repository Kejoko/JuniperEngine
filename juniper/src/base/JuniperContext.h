#pragma once

#include <memory>
#include <vector>

#include "Core.h"

namespace jun {
    class JuniperContext;
}

/**
 * @brief This is where all of the grimy "low level" stuff lives. When I say
 * low level, I mean window interface stuff. Stuff that we want to abstract
 * away so we don't need to think about it later.
 * 
 * @details These are all shared pointers to things so they can be declared
 * when the JuniperBase instance is created. JuniperBase owns the
 * JuniperContext and all of the other base objects use these pointers for
 * their functionality. 
 * 
 */
class jun::JuniperContext {
    public:
        JuniperContext() = default;
        ~JuniperContext() = default;

        // JuniperWindow, JuniperSurface, JuniperDevice, JuniperSwapChain
        std::shared_ptr<GLFWwindow*> mppWindow = std::make_shared<GLFWwindow*>();

        // JuniperVkInstance, JuniperSurface, JuniperDevice
        std::shared_ptr<VkInstance> mpInstance = std::make_shared<VkInstance>();

        // JuniperSurface, JuniperDevice, JuniperSwapChain, JuniperCommandBuffer
        std::shared_ptr<VkSurfaceKHR> mpSurface = std::make_shared<VkSurfaceKHR>();
        
        // JuniperDevice, JuniperSwapChain, JuniperCommandBuffer
        std::shared_ptr<VkPhysicalDevice> mpPhysicalDevice = std::make_shared<VkPhysicalDevice>();
        
        // JuniperDevice, JuniperSwapChain, JuniperPipeline, JuniperFrameBuffer, JuniperCommandBuffer
        std::shared_ptr<VkDevice> mpDevice = std::make_shared<VkDevice>();

        // JuniperDevice
        std::shared_ptr<VkQueue> mpGraphicsQueue = std::make_shared<VkQueue>();
        std::shared_ptr<VkQueue> mpPresentQueue = std::make_shared<VkQueue>();
        
        // JuniperSwapChain
        std::shared_ptr<VkSwapchainKHR> mpSwapChain = std::make_shared<VkSwapchainKHR>();
        std::shared_ptr<std::vector<VkImage>> mpSwapChainImages = std::make_shared<std::vector<VkImage>>();
        
        // JuniperSwapChain, JuniperPipeline
        std::shared_ptr<VkFormat> mpSwapChainImageFormat = std::make_shared<VkFormat>();
        
        // JuniperSwapChain, JuniperPipeline, JuniperFrameBuffer, JuniperCommandBuffer
        std::shared_ptr<VkExtent2D> mpSwapChainExtent = std::make_shared<VkExtent2D>();
        
        // JuniperSwapChain, JuniperFrameBuffer
        std::shared_ptr<std::vector<VkImageView>> mpSwapChainImageViews = std::make_shared<std::vector<VkImageView>>();
        
        // JuniperPipeline, JuniperFrameBuffer, JuniperCommandBuffer
        std::shared_ptr<VkRenderPass> mpRenderPass = std::make_shared<VkRenderPass>();
        
        // JuniperPipeline
        std::shared_ptr<VkPipelineLayout> mpPipelineLayout = std::make_shared<VkPipelineLayout>();
        
        // JuniperPipeline, JuniperCommandBuffer
        std::shared_ptr<VkPipeline> mpGraphicsPipeline = std::make_shared<VkPipeline>();
        
        // JuniperFrameBuffer, JuniperCommandBuffer
        std::shared_ptr<std::vector<VkFramebuffer>> mpSwapChainFramebuffers = std::make_shared<std::vector<VkFramebuffer>>();
        std::shared_ptr<VkCommandPool> mpCommandPool = std::make_shared<VkCommandPool>();
        std::shared_ptr<std::vector<VkCommandBuffer>> mpCommandBuffers = std::make_shared<std::vector<VkCommandBuffer>>();
};