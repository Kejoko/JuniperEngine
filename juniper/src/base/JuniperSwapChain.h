#pragma once

#include "Core.h"
#include "JuniperDevice.h"

#include <memory>

namespace jun {
    class JuniperSwapChain;
}

class jun::JuniperSwapChain {
    public:
        JuniperSwapChain(std::shared_ptr<GLFWwindow*> ppWindow,
                         std::shared_ptr<VkSurfaceKHR> pSurface,
                         std::shared_ptr<VkPhysicalDevice> pPhysicalDevice,
                         std::shared_ptr<VkDevice> pDevice);
        ~JuniperSwapChain() = default;

        void cleanup();
    private:
        std::shared_ptr<GLFWwindow*> mppWindow;
        std::shared_ptr<VkSurfaceKHR> mpSurface;
        std::shared_ptr<VkPhysicalDevice> mpPhysicalDevice;
        std::shared_ptr<VkDevice> mpDevice;
        VkSwapchainKHR mSwapChain;
        std::vector<VkImage> mSwapChainImages;
        VkFormat mSwapChainImageFormat;
        VkExtent2D mSwapChainExtent;


        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void createSwapChain();
};