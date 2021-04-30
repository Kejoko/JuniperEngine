#pragma once

#include "Core.h"
#include "JuniperVkInstance.h"
#include "JuniperWindow.h"

#include <string>

namespace jun {
    class JuniperBase;
}

class jun::JuniperBase {
    public:
        JuniperBase(int major, int minor, int patch, int width, int height, std::string name);
        ~JuniperBase();

        void run();
        void cleanup();

    private:
        // Todo: make app info struct to hold version info and name
        const int mMajorVersion;
        const int mMinorVersion;
        const int mPatchVersion;
        int mWidth;
        int mHeight;
        std::string mName;
        JuniperWindow mJWindow;
        JuniperVkInstance mJVkInstance;

        void mainLoop();
};