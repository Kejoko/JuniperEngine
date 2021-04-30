#include "JuniperVkInstance.h"

#include "Core.h"

#include <stdexcept>
#include <string>
#include <vector>

void jun::JuniperVkInstance::init(int major, int minor, int patch, std::string name) {
    jun::Logger::trace("Initializing JuniperVkInstance");
    mMajorVersion = major;
    mMinorVersion = major;
    mPatchVersion = major;
    mName = name;
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

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS) {
        jun::Logger::critical("Failed to create vulkan instance.");
        throw std::runtime_error("Failed to create vulkan instance.");
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    jun::Logger::info("Available Vulkan extensions:");
    for (const VkExtensionProperties& extension : extensions) {
        jun::Logger::info(std::string("\t") + std::string(extension.extensionName));
    }
}
