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

/**
 * @brief The base object for the Juniper Engine. You construct one of these
 * with the relevant application information. This is what the entire engine,
 * and thusly the game, is built on.
 */
class jun::JuniperBase {
    public:
        JuniperBase(const AppInfo& appInfo, int windowWidth, int windowHeight);
        ~JuniperBase();

        void run();
        void cleanup();

    private:
        /**
         * @todo Just store the appInfo object instead of copying all of the
         * information to here
         */
        const std::string mName;
        const int mMajorVersion;
        const int mMinorVersion;
        const int mPatchVersion;
        
        int mWindowWidth;
        int mWindowHeight;
        
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