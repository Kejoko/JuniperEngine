#include "JuniperBase.h"

#include "Core.h"
#include "AppInfo.h"
#include "JuniperContext.h"
#include "JuniperDevice.h"
#include "JuniperSurface.h"
#include "JuniperSwapChain.h"
#include "JuniperVkInstance.h"
#include "JuniperWindow.h"

#include <memory>
#include <string>


jun::JuniperBase::JuniperBase(const AppInfo& info, int width, int height) :
                              mMajorVersion{info.mMajorVersion},
                              mMinorVersion{info.mMinorVersion},
                              mPatchVersion{info.mPatchVersion},
                              mName{info.mName},
                              mWidth{width},
                              mHeight{height},
                              mJContext{},
                              mJWindow{mWidth, mHeight, mName, mJContext},
                              mJVkInstance{info, mJContext},
                              mJSurface{mJContext},
                              mJDevice{info, mJContext},
                              mJSwapChain{mJContext},
                              mJPipeline{mJContext} {
    jun::Logger::trace("JuniperBase initialized");
}

jun::JuniperBase::~JuniperBase() {
    cleanup();
}

void jun::JuniperBase::run() {
    mainLoop();
}

void jun::JuniperBase::cleanup() {
    jun::Logger::trace("Cleaning up JuniperBase");

    mJPipeline.cleanup();
    mJSwapChain.cleanup();
    mJDevice.cleanup();
    mJSurface.cleanup();
    mJVkInstance.cleanup();
    mJWindow.cleanup();
}

void jun::JuniperBase::mainLoop() {
    while (!mJWindow.shouldClose()) {
        glfwPollEvents();
    }
}