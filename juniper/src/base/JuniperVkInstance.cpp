#include "JuniperVkInstance.h"

#include "Core.h"
#include "AppInfo.h"

#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

void jun::JuniperVkInstance::init(const AppInfo& info) {
    jun::Logger::trace("Initializing JuniperVkInstance");
    mMajorVersion = info.mMajorVersion;
    mMinorVersion = info.mMinorVersion;
    mPatchVersion = info.mPatchVersion;
    mName = info.mName;

    #if defined(BUILD_DEBUG)
    mValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
    mEnableValidationLayers = true;
    #else
    mEnableValidationLayers = false;
    #endif

    createInstance();
    jun::Logger::trace("JuniperVKInstance initialized");
}

void jun::JuniperVkInstance::cleanup() {
    jun::Logger::trace("Cleaning up JuniperVkInstance");
    vkDestroyInstance(mInstance, nullptr);
    jun::Logger::trace("JuniperVkInstance cleaned up");
}

void jun::JuniperVkInstance::createInstance() {
    jun::Logger::trace("Creating Vulkan instance");
    
    // Check for the required validation layers
    if (mEnableValidationLayers) {
        verifyValidationLayerSupport();
    }

    // Tell the driver the information about our app
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = mName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(mMajorVersion, mMajorVersion, mMajorVersion);
    appInfo.pEngineName = JUN_NAME_STRING;
    appInfo.engineVersion = VK_MAKE_VERSION(JUN_MAJOR_VERSION, JUN_MINOR_VERSION, JUN_PATCH_VERSION);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Determine which extensions glfw requires and list them
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> requiredExtensions;
    for (size_t i = 0; i < glfwExtensionCount; i++) {
        requiredExtensions.push_back(glfwExtensions[i]);
    }
    #if defined(BUILD_DEBUG)
    requiredExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    #endif

    // Determine which extensions are available and list them
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    if (extensionCount == 0) {
        jun::Logger::critical("No extensions available.");
        throw std::runtime_error("No extensions available.");
    }

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    
    jun::Logger::info("Available Vulkan extensions:");
    for (const VkExtensionProperties& extension : extensions) {
        jun::Logger::info(std::string("\t") + std::string(extension.extensionName));
    }

    // Ensure all of the required extensions are available
    // Todo

    // Tell the driver which global extensions and validation layers we would like to use
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    if (mEnableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
        createInfo.ppEnabledLayerNames = mValidationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    // Actually try to create the instance
    if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS) {
        jun::Logger::critical("Failed to create Vulkan instance.");
        throw std::runtime_error("Failed to create Vulkan instance.");
    }
}

void jun::JuniperVkInstance::verifyValidationLayerSupport() {
    jun::Logger::trace("Verifying validation layer support");

    // List the desired validation layers
    jun::Logger::info("Requested validation layers:");
    for (const char* layerName : mValidationLayers) {
        jun::Logger::info(std::string("\t") + std::string(layerName));
    }

    // Get all of the available validation layers and list them
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    jun::Logger::info("Available validation layers:");
    for (VkLayerProperties layerProperties : availableLayers) {
        jun::Logger::info(std::string("\t") + std::string(layerProperties.layerName));
    }

    // Ensure all of the desired layers are available
    for (const char* layerName : mValidationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            jun::Logger::critical("Requested validation layer is not available: " + std::string(layerName));
            throw std::runtime_error("Requested validation layer is not available: " + std::string(layerName));
        }
    }
}