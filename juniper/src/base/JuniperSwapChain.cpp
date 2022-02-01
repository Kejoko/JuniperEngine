#include "JuniperSwapChain.h"

#include "Core.h"
#include "JuniperContext.h"
#include "JuniperDevice.h"

jun::JuniperSwapChain::JuniperSwapChain(const JuniperContext& context) :
    mppWindow(context.mppWindow),
    mpSurface(context.mpSurface),
    mpPhysicalDevice(context.mpPhysicalDevice),
    mpDevice(context.mpDevice),
    mpSwapChain(context.mpSwapChain),
    mpSwapChainImages(context.mpSwapChainImages),
    mpSwapChainImageFormat(context.mpSwapChainImageFormat),
    mpSwapChainExtent(context.mpSwapChainExtent),
    mpSwapChainImageViews(context.mpSwapChainImageViews)
{
    createSwapChain();
    createImageViews();

    jun::Logger::trace("JuniperSwapChain initialized");
}

void jun::JuniperSwapChain::cleanup() {
    jun::Logger::trace("Cleaning up JuniperSwapChain");

    for (VkImageView imageView : *mpSwapChainImageViews) {
        vkDestroyImageView(*mpDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(*mpDevice, *mpSwapChain, nullptr);
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
    createInfo.imageArrayLayers = 1;    // Always 1 unless 3d stereoscopic (vr)
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

    if (vkCreateSwapchainKHR(*mpDevice, &createInfo, nullptr, mpSwapChain.get()) != VK_SUCCESS) {
        jun::Logger::critical("Failed to create swap chain");
        throw std::runtime_error("Failed to create swap chain");
    }

    vkGetSwapchainImagesKHR(*mpDevice, *mpSwapChain, &imageCount, nullptr);
    mpSwapChainImages->resize(imageCount);
    vkGetSwapchainImagesKHR(*mpDevice, *mpSwapChain, &imageCount, mpSwapChainImages->data());

    *mpSwapChainImageFormat = surfaceFormat.format;
    *mpSwapChainExtent = extent;
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

void jun::JuniperSwapChain::createImageViews() {
    mpSwapChainImageViews->resize(mpSwapChainImages->size());

    for (size_t i = 0; i < mpSwapChainImages->size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = (*mpSwapChainImages)[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = *mpSwapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(*mpDevice, &createInfo, nullptr, &(*mpSwapChainImageViews)[i]) != VK_SUCCESS) {
            jun::Logger::critical("Failed to create image views");
            throw std::runtime_error("Failed to create image views");
        }
    }
}