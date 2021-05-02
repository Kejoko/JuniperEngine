#pragma once

#include "Core.h"
#include "AppInfo.h"

#include <memory>
#include <string>

namespace jun {
    class JuniperVkInstance;
}

class jun::JuniperVkInstance {
    public:
        JuniperVkInstance(const AppInfo& info, std::shared_ptr<VkInstance> pInstance);
        ~JuniperVkInstance() = default;

        JuniperVkInstance(const JuniperVkInstance&) = delete;
        JuniperVkInstance& operator=(const JuniperVkInstance&) = delete;

        void cleanup();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

    private:
        const std::vector<const char*> mValidationLayers = {"VK_LAYER_KHRONOS_validation"};
        #if defined(BUILD_DEBUG)
        const bool mEnableValidationLayers = true;
        #else
        const bool mEnableValidationLayers = false;
        #endif

        int mMajorVersion;
        int mMinorVersion;
        int mPatchVersion;
        std::string mName;
        
        std::shared_ptr<VkInstance> mpInstance;
        VkDebugUtilsMessengerEXT mDebugMessenger;

        void createInstance();
        void verifyValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
        void setupDebugMessenger();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        VkResult createDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                              const VkAllocationCallbacks* pAllocator,
                                              VkDebugUtilsMessengerEXT* pDebugMessenger);
        void destroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator);
};