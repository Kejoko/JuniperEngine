#pragma once

#include "Core.h"
#include "JuniperContext.h"
#include "JuniperDevice.h"

#include <memory>

namespace jun {
    class JuniperSwapChain;
}

class jun::JuniperSwapChain {
    public:
        JuniperSwapChain(const JuniperContext& context);
        ~JuniperSwapChain() = default;

        void cleanup();
    private:
        std::shared_ptr<GLFWwindow*> mppWindow;
        std::shared_ptr<VkSurfaceKHR> mpSurface;
        std::shared_ptr<VkPhysicalDevice> mpPhysicalDevice;
        std::shared_ptr<VkDevice> mpDevice;
        std::shared_ptr<VkSwapchainKHR> mpSwapChain;
        std::shared_ptr<std::vector<VkImage>> mpSwapChainImages;
        std::shared_ptr<VkFormat> mpSwapChainImageFormat;
        std::shared_ptr<VkExtent2D> mpSwapChainExtent;
        std::shared_ptr<std::vector<VkImageView>> mpSwapChainImageViews;

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void createSwapChain();
        void createImageViews();
};