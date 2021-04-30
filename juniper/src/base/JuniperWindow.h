#pragma once

#include "Core.h"

#include <string>

namespace jun {
    class JuniperWindow;
}

class jun::JuniperWindow {
    public:
        JuniperWindow() = default;
        ~JuniperWindow() = default;

        JuniperWindow(const JuniperWindow&) = delete;
        JuniperWindow& operator=(const JuniperWindow&) = delete;

        void init(int width, int height, std::string name);
        void cleanup();

        bool shouldClose();
    private:
        int mWidth;
        int mHeight;
        std::string mName;
        GLFWwindow* mpWindow;
};