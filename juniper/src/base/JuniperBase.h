#pragma once

// TODO
// Make the shared pointers inside of a class/struct called JuniperContext
// Pass this context around to the constructors instead of passing so many goddamned parameters

#include "Core.h"
#include "AppInfo.h"
#include "JuniperDevice.h"
#include "JuniperSurface.h"
#include "JuniperSwapChain.h"
#include "JuniperVkInstance.h"
#include "JuniperWindow.h"

#include <memory>
#include <string>

namespace jun {
    class JuniperBase;
}

class jun::JuniperBase {
    public:
        JuniperBase(const AppInfo& appInfo, int width, int height);
        ~JuniperBase();

        void run();
        void cleanup();

    private:
        const int mMajorVersion;
        const int mMinorVersion;
        const int mPatchVersion;
        const std::string mName;
        
        int mWidth;
        int mHeight;

        std::shared_ptr<VkInstance> mpInstance;
        std::shared_ptr<GLFWwindow*> mppWindow;
        std::shared_ptr<VkSurfaceKHR> mpSurface;
        std::shared_ptr<VkPhysicalDevice> mpPhysicalDevice;
        std::shared_ptr<VkDevice> mpDevice;
        std::shared_ptr<VkQueue> mpGraphicsQueue;
        std::shared_ptr<VkQueue> mpPresentQueue;
        JuniperWindow mJWindow;
        JuniperVkInstance mJVkInstance;
        JuniperSurface mJSurface;
        JuniperDevice mJDevice;
        JuniperSwapChain mJSwapChain;

        void mainLoop();
};