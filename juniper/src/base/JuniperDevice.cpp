#include "JuniperDevice.h"

#include "Core.h"

#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

jun::JuniperDevice::JuniperDevice(std::shared_ptr<VkInstance> pInstance) :
                                  mpInstance{pInstance},
                                  mPhysicalDevice{VK_NULL_HANDLE} {
    pickPhysicalDevice();

    jun::Logger::trace("JuniperDevice initialized");
}

void jun::JuniperDevice::cleanup() {

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
        mPhysicalDevice = candidates.rbegin()->second;
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
    if (!findQueueFamilies(device).isComplete()) {
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

jun::JuniperDevice::QueueFamilyIndices jun::JuniperDevice::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    // Get the available queue families
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    // Ensure that at least one of these queue families VK_QUEUE_GRAPHICS_BIT
    for (int i = 0; i < queueFamilies.size(); i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.mGraphicsFamily = i;
        }
    }

    return indices;
}

bool jun::JuniperDevice::QueueFamilyIndices::isComplete() {
    return mGraphicsFamily.has_value();
}