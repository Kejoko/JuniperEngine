#include "JuniperSwapChain.h"

#include "Core.h"
#include "JuniperContext.h"
#include "JuniperDevice.h"

jun::JuniperSwapChain::JuniperSwapChain(const JuniperContext& context) :
                                        mppWindow{context.mppWindow},
                                        mpSurface{context.mpSurface},
                                        mpDevice{context.mpDevice},
                                        mpPhysicalDevice{context.mpPhysicalDevice} {
    createSwapChain();

    jun::Logger::trace("JuniperSwapChain initialized");
}

void jun::JuniperSwapChain::cleanup() {
    jun::Logger::trace("Cleaning up JuniperSwapChain");

    vkDestroySwapchainKHR(*mpDevice, mSwapChain, nullptr);
}

void jun::JuniperSwapChain::createSwapChain() {
    SwapChainSupportDetails swapChainSupport;
    swapChainSupport.querySwapChainSupport(*mpPhysicalDevice, *mpSurface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.mFormats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.mPresentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.mCapabilities);

    // maxImageCount == 0 means there is not max image count
    uint32_t imageCount = swapChainSupport.mCapabilities.minImageCount + 1;
    if (swapChainSupport.mCapabilities.maxImageCount > 0 && imageCount > swapChainSupport.mCapabilities.maxImageCount) {
        imageCount = swapChainSupport.mCapabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = *mpSurface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;    // Always 1 unless 3d stereoscopic
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // Possible to render images to separate image first to perform operations like post-processing (use different bit)

    QueueFamilyIndices indices;
    indices.findQueueFamilies(*mpPhysicalDevice, *mpSurface);
    uint32_t queueFamilyIndices[] = {indices.mGraphicsFamily.value(), indices.mPresentFamily.value()};

    if (indices.mGraphicsFamily != indices.mPresentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Images can be used across multiple queue families without explicit ownership transfers
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Image owned by one queue family at a time. ownership must be explicitly transferred before using it in another queue family. Best performance
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.mCapabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(*mpDevice, &createInfo, nullptr, &mSwapChain) != VK_SUCCESS) {
        jun::Logger::critical("Failed to create swap chain");
        throw std::runtime_error("Failed to create swap chain");
    }

    vkGetSwapchainImagesKHR(*mpDevice, mSwapChain, &imageCount, nullptr);
    mSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(*mpDevice, mSwapChain, &imageCount, mSwapChainImages.data());

    mSwapChainImageFormat = surfaceFormat.format;
    mSwapChainExtent = extent;
}

VkSurfaceFormatKHR jun::JuniperSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR jun::JuniperSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D jun::JuniperSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(*mppWindow, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}