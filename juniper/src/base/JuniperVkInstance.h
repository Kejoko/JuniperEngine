#pragma once

#include <memory>
#include <string>

#include "Core.h"
#include "AppInfo.h"
#include "JuniperContext.h"

namespace jun {
    class JuniperVkInstance;
}

/**
 * @brief The interface to the Vulkan Instance, so we may actually talk
 * to the graphics card.
 */
class jun::JuniperVkInstance {
    public:
        JuniperVkInstance(const AppInfo& appInfo, const JuniperContext& context);
        ~JuniperVkInstance() = default;

        JuniperVkInstance(const JuniperVkInstance&) = delete;
        JuniperVkInstance& operator=(const JuniperVkInstance&) = delete;

        void cleanup();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
        );

    private:
        /**
         * @todo JUST SAVE THE APP INFO >:(
         */
        std::string mName;
        int mMajorVersion;
        int mMinorVersion;
        int mPatchVersion;
        
        const std::vector<const char*> mValidationLayers;
        const bool mEnableValidationLayers;
        std::shared_ptr<VkInstance> mpInstance;
        VkDebugUtilsMessengerEXT mDebugMessenger;

        void createInstance();
        void verifyValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
        void setupDebugMessenger();
        void populateDebugMessengerCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT& createInfo
        );
        VkResult createDebugUtilsMessengerEXT(
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger
        );
        void destroyDebugUtilsMessengerEXT(
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator
        );
};