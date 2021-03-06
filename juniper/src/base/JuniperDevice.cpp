#include "JuniperDevice.h"

#include "Core.h"
#include "AppInfo.h"
#include "JuniperContext.h"

#include <cstdint>
#include <algorithm>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

jun::JuniperDevice::JuniperDevice(const AppInfo& info, const JuniperContext& context) :
                                  mValidationLayers{info.mValidationLayers},
                                  mDeviceExtensions{info.mDeviceExtensions},
                                  mEnableValidationLayers{info.mEnableValidationLayers},
                                  mpInstance{context.mpInstance},
                                  mppWindow{context.mppWindow},
                                  mpSurface{context.mpSurface},
                                  mpPhysicalDevice{context.mpPhysicalDevice},
                                  mpDevice{context.mpDevice},
                                  mpGraphicsQueue{context.mpGraphicsQueue},
                                  mpPresentQueue{context.mpPresentQueue} {
    *mpPhysicalDevice = VK_NULL_HANDLE;

    pickPhysicalDevice();
    createLogicalDevice();

    jun::Logger::trace("JuniperDevice initialized");
}

void jun::JuniperDevice::cleanup() {
    jun::Logger::trace("Cleaning up JuniperDevice");

    vkDestroyDevice(*mpDevice, nullptr);
}

void jun::JuniperDevice::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(*mpInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        jun::Logger::critical("Unable to find a GPU with Vulkan support");
        throw std::runtime_error("Unable to find a GPU with Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(*mpInstance, &deviceCount, devices.data());

    // Use an ordered map to automatically sort candidates by increasing score
    std::multimap<int, VkPhysicalDevice> candidates;

    jun::Logger::info("Physical device candidates and their scores:");
    for (const auto& device : devices) {
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }

    // Ensure that the best candidate really is suitable
    if (candidates.rbegin()->first > 0) {
        // mPhysicalDevice = candidates.rbegin()->second;
        *mpPhysicalDevice = candidates.rbegin()->second;
    } else {
        jun::Logger::critical("Failed to find a suitable GPU");
        throw std::runtime_error("Failed to find a suitable GPU");
    }
}

int jun::JuniperDevice::rateDeviceSuitability(VkPhysicalDevice device) {
    // Get the properties and supported features for this device
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    int score = 0;

    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    // Application can't function without geometry shaders
    #ifndef __APPLE__
    if (!deviceFeatures.geometryShader) {
        score = 0;
    }
    #endif

    // Ensure all of the required queue families are available for this device
    bool extensionsSupported = checkDeviceExtensionSupport(device);
    QueueFamilyIndices indices;
    indices.findQueueFamilies(device, *mpSurface);
    if (!(indices.isComplete() && extensionsSupported)) {
        score = 0;
    }

    // Ensure that the available swap chain has all of the features that we need
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport;
        swapChainSupport.querySwapChainSupport(device, *mpSurface);
        swapChainAdequate = !swapChainSupport.mFormats.empty() && !swapChainSupport.mPresentModes.empty();
    }
    if (!swapChainAdequate) {
        score = 0;
    }

    // Report the information about this device
    std::string report = deviceProperties.deviceName + std::string(" (");
    if (deviceProperties.deviceType == 0) {
        report += "other";
    } else if (deviceProperties.deviceType == 1) {
        report += "integrated";
    } else if (deviceProperties.deviceType == 2) {
        report += "discrete";
    } else if (deviceProperties.deviceType == 3) {
        report += "virtual";
    } else if (deviceProperties.deviceType == 4) {
        report += "cpu";
    }
    report += ") - " + std::to_string(score);
    jun::Logger::info("\t" + report);

    return score;
}

bool jun::JuniperDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    // Get all of the available device extensions
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    // Ensure that all of the required extensions are available
    for (const char* requiredExtension : mDeviceExtensions) {
        bool extensionSupported = false;

        for (const auto& availableExtension : availableExtensions) {
            if (strcmp(requiredExtension, availableExtension.extensionName) == 0) {
                extensionSupported = true;
                break;
            }
        }

        if (!extensionSupported) {
            return false;
        }
    }

    return true;
}

// Todo
// Add logic to prefer a device which supports drawing and presentation in the same queue for performance benefits

void jun::JuniperDevice::createLogicalDevice() {
    QueueFamilyIndices indices;
    indices.findQueueFamilies(*mpPhysicalDevice, *mpSurface);

    // Determine the unique queues we must create
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.mGraphicsFamily.value(), indices.mPresentFamily.value()};

    // Create all of the unique queues
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Specify which device features we want to use
    VkPhysicalDeviceFeatures deviceFeatures{};

    // Specify information, extensions, and validation layers for driver
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(mDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = mDeviceExtensions.data();

    if (mEnableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
        createInfo.ppEnabledLayerNames = mValidationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    // Create the device and get its device queue
    // if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) != VK_SUCCESS) {
    if (vkCreateDevice(*mpPhysicalDevice, &createInfo, nullptr, mpDevice.get()) != VK_SUCCESS) {
        jun::Logger::critical("Failed to create logical device");
        throw std::runtime_error("Failed to create logical device");
    }

    // vkGetDeviceQueue(mDevice, indices.mGraphicsFamily.value(), 0, &mGraphicsQueue);
    // vkGetDeviceQueue(mDevice, indices.mPresentFamily.value(), 0, &mGraphicsQueue);
    vkGetDeviceQueue(*mpDevice, indices.mGraphicsFamily.value(), 0, mpGraphicsQueue.get());
    vkGetDeviceQueue(*mpDevice, indices.mPresentFamily.value(), 0, mpGraphicsQueue.get());
}

void jun::QueueFamilyIndices::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    // Get the available queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    // Ensure that at least one of these queue families VK_QUEUE_GRAPHICS_BIT
    for (int i = 0; i < queueFamilies.size(); i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            mGraphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
            mPresentFamily = i;
        }

        if (isComplete()) {
            break;
        }
    }
}

bool jun::QueueFamilyIndices::isComplete() {
    return mGraphicsFamily.has_value() && mPresentFamily.has_value();
}

void jun::SwapChainSupportDetails::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &mCapabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if (formatCount != 0) {
            mFormats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, mFormats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            mPresentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, mPresentModes.data());
        }
}