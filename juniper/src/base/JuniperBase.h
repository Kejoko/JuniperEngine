#pragma once

#include "Core.h"
#include "JuniperWindow.h"

#include <string>

namespace jun {
    class JuniperBase;
}

class jun::JuniperBase {
    public:
        JuniperBase(int width, int height, std::string name);
        ~JuniperBase();

        void run();
        void cleanup();

    private:
        int mWidth;
        int mHeight;
        std::string mName;
        JuniperWindow mJWindow;
        // Some member for the vulkan stuff as in the initVulkan() function from vulkan tutorial

        void mainLoop();
};