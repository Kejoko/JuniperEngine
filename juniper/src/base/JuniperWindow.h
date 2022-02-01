#pragma once

#include <memory>
#include <string>

#include "Core.h"
#include "JuniperContext.h"

namespace jun {
    class JuniperWindow;
}

/**
 * @brief The window we render things to :)
 * 
 */
class jun::JuniperWindow {
    public:
        JuniperWindow(
            int windowWidth,
            int windowHeight,
            const std::string& name,
            const JuniperContext& context
        );
        ~JuniperWindow() = default;

        JuniperWindow(const JuniperWindow&) = delete;
        JuniperWindow& operator=(const JuniperWindow&) = delete;

        void cleanup();

        bool shouldClose();
    private:
        int mWindowWidth;
        int mWindowHeight;
        std::string mName;
        std::shared_ptr<GLFWwindow*> mppWindow;
};