#pragma once

#include "Core.h"

#include <memory>

namespace jun {
    class JuniperSurface;
}

class jun::JuniperSurface {
    public:
        JuniperSurface(std::shared_ptr<VkInstance> pInstance, std::shared_ptr<GLFWwindow*> ppWindow, std::shared_ptr<VkSurfaceKHR> pSurface);
        ~JuniperSurface() = default;

        void cleanup();
    private:
        std::shared_ptr<VkInstance> mpInstance;
        std::shared_ptr<GLFWwindow*> mppWindow;
        std::shared_ptr<VkSurfaceKHR> mpSurface;

        void createSurface();
};