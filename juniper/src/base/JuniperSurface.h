#pragma once

#include "Core.h"
#include "JuniperContext.h"

#include <memory>

namespace jun {
    class JuniperSurface;
}

class jun::JuniperSurface {
    public:
        JuniperSurface(const JuniperContext& context);
        ~JuniperSurface() = default;

        void cleanup();
    private:
        std::shared_ptr<VkInstance> mpInstance;
        std::shared_ptr<GLFWwindow*> mppWindow;
        std::shared_ptr<VkSurfaceKHR> mpSurface;

        void createSurface();
};