#pragma once

#include "Core.h"
#include "AppInfo.h"
#include "JuniperDevice.h"
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
        JuniperWindow mJWindow;
        JuniperVkInstance mJVkInstance;
        JuniperDevice mJDevice;

        void mainLoop();
};