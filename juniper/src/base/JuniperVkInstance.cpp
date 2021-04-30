#include "JuniperVkInstance.h"

#include "Core.h"
#include "AppInfo.h"

#include <stdexcept>
#include <string>
#include <vector>

void jun::JuniperVkInstance::init(const AppInfo& info) {
    jun::Logger::trace("Initializing JuniperVkInstance");
    mMajorVersion = info.mMajorVersion;
    mMinorVersion = info.mMinorVersion;
    mPatchVersion = info.mPatchVersion;
    mName = info.mName;
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
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = mName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(mMajorVersion, mMajorVersion, mMajorVersion);
    appInfo.pEngineName = JUN_NAME_STRING;
    appInfo.engineVersion = VK_MAKE_VERSION(JUN_MAJOR_VERSION, JUN_MINOR_VERSION, JUN_PATCH_VERSION);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Determine which extensions glfw requires
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

    // Determine which extensions are available
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
    createInfo.enabledLayerCount = 0;

    // Actually try to create the instance
    if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS) {
        jun::Logger::critical("Failed to create Vulkan instance.");
        throw std::runtime_error("Failed to create Vulkan instance.");
    }
}
