#include "JuniperVkInstance.h"

#include "Core.h"
#include "AppInfo.h"

#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

jun::JuniperVkInstance::JuniperVkInstance(const AppInfo& info) :
                                          mMajorVersion{info.mMajorVersion},
                                          mMinorVersion{info.mMinorVersion},
                                          mPatchVersion{info.mPatchVersion},
                                          mName{info.mName} {
    #if defined(BUILD_DEBUG)
    mValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
    mEnableValidationLayers = true;
    #else
    mEnableValidationLayers = false;
    #endif

    createInstance();
    setupDebugMessenger();

    jun::Logger::trace("JuniperVKInstance initialized");
}

void jun::JuniperVkInstance::cleanup() {
    jun::Logger::trace("Cleaning up JuniperVkInstance");

    destroyDebugUtilsMessengerEXT(mDebugMessenger, nullptr);
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

    // Determine which extensions are required
    std::vector<const char*> requiredExtensions = getRequiredExtensions();

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
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (mEnableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(mValidationLayers.size());
        createInfo.ppEnabledLayerNames = mValidationLayers.data();

        // Allow for debugging messages in the VkCreateInstance and VkDestroyInstance call
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
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

std::vector<const char*> jun::JuniperVkInstance::getRequiredExtensions() {
    // Determine which extensions glfw requires
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    for (size_t i = 0; i < glfwExtensionCount; i++) {
        extensions.push_back(glfwExtensions[i]);
    }
    
    // If we are debugging add the printing extension
    if (mEnableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    // List the required extensions
    jun::Logger::info("Required Vulkan Extensions:");
    for (const char* extension : extensions) {
        jun::Logger::info(std::string("\t") + std::string(extension));
    }
    
    return extensions;
}

void jun::JuniperVkInstance::setupDebugMessenger() {
    if (!mEnableValidationLayers) return;

    jun::Logger::trace("Setting up Vulkan debug messenger");

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (createDebugUtilsMessengerEXT(&createInfo, nullptr, &mDebugMessenger) != VK_SUCCESS) {
        jun::Logger::critical("Failed to set up Vulkan debug messenger.");
        throw std::runtime_error("Failed to set up Vulkan debug messenger.");
    }
}

void jun::JuniperVkInstance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    jun::Logger::trace("Populating debug messenger create info");
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
}

VkResult jun::JuniperVkInstance::createDebugUtilsMessengerEXT(
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger) {
    jun::Logger::trace("Loading Vulkan function to create the debug messenger");
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        jun::Logger::trace("Successfully loaded function. Creating debug messenger");
        return func(mInstance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        jun::Logger::critical("Unable to load function");
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void jun::JuniperVkInstance::destroyDebugUtilsMessengerEXT(
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator) {
    if (!mEnableValidationLayers) return;

    jun::Logger::trace("Loading function to destroy debug messenger");

    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        jun::Logger::trace("Successfully loaded function. Destroying debug messenger");
        func(mInstance, mDebugMessenger, pAllocator);
    } else {
        jun::Logger::warn("Unable to load function. Unable to destroy debug messenger");
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL jun::JuniperVkInstance::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

        std::string message = "Validation layer: ";
        message += pCallbackData->pMessage;

        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
            jun::Logger::debug(message);
        } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            jun::Logger::info(message);
        } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            jun::Logger::warn(message);
        } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            jun::Logger::error(message);
        }

        // Return false to denote that the validation layer should NOT be aborted
        return VK_FALSE;
    }