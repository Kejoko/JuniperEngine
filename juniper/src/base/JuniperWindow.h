#pragma once

#include "Core.h"
#include "JuniperContext.h"

#include <memory>
#include <string>

namespace jun {
    class JuniperWindow;
}

class jun::JuniperWindow {
    public:
        JuniperWindow(int width, int height, const std::string& name, const JuniperContext& context);
        ~JuniperWindow() = default;

        JuniperWindow(const JuniperWindow&) = delete;
        JuniperWindow& operator=(const JuniperWindow&) = delete;

        void cleanup();

        bool shouldClose();
    private:
        int mWidth;
        int mHeight;
        std::string mName;
        std::shared_ptr<GLFWwindow*> mppWindow;
};