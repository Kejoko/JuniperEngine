#pragma once

#include "Core.h"
#include "AppInfo.h"
#include "JuniperVkInstance.h"
#include "JuniperWindow.h"

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
        JuniperWindow mJWindow;
        JuniperVkInstance mJVkInstance;

        void mainLoop();
};