#pragma once

// TODO
// Make the shared pointers inside of a class/struct called JuniperContext
// Pass this context around to the constructors instead of passing so many goddamned parameters

#include "Core.h"
#include "AppInfo.h"
#include "JuniperContext.h"
#include "JuniperCommandBuffer.h"
#include "JuniperDevice.h"
#include "JuniperFrameBuffer.h"
#include "JuniperPipeline.h"
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
        
        JuniperContext mJContext;
        JuniperWindow mJWindow;
        JuniperVkInstance mJVkInstance;
        JuniperSurface mJSurface;
        JuniperDevice mJDevice;
        JuniperSwapChain mJSwapChain;
        JuniperPipeline mJPipeline;
        JuniperFrameBuffer mJFrameBuffer;
        JuniperCommandBuffer mJCommandBuffer;

        void mainLoop();
};