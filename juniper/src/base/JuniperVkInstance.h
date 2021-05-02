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
        int mMajorVersion;
        int mMinorVersion;
        int mPatchVersion;
        std::string mName;
        
        const std::vector<const char*> mValidationLayers;
        const bool mEnableValidationLayers;
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