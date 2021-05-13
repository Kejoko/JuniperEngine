#pragma once

#include "Core.h"

#include <memory>

namespace jun {
    class JuniperContext;
}

class jun::JuniperContext {
    public:
        JuniperContext() = default;
        ~JuniperContext() = default;

        std::shared_ptr<VkInstance> mpInstance = std::make_shared<VkInstance>();
        std::shared_ptr<GLFWwindow*> mppWindow = std::make_shared<GLFWwindow*>();
        std::shared_ptr<VkSurfaceKHR> mpSurface = std::make_shared<VkSurfaceKHR>();
        std::shared_ptr<VkPhysicalDevice> mpPhysicalDevice = std::make_shared<VkPhysicalDevice>();
        std::shared_ptr<VkDevice> mpDevice = std::make_shared<VkDevice>();
        std::shared_ptr<VkQueue> mpGraphicsQueue = std::make_shared<VkQueue>();
        std::shared_ptr<VkQueue> mpPresentQueue = std::make_shared<VkQueue>();
};