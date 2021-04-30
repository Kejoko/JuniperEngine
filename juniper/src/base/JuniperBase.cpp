#include "JuniperBase.h"

#include "Core.h"
#include "AppInfo.h"
#include "JuniperVkInstance.h"
#include "JuniperWindow.h"

#include <string>

jun::JuniperBase::JuniperBase(const AppInfo& info, int width, int height) :
                              mMajorVersion{info.mMajorVersion}, mMinorVersion{info.mMinorVersion}, mPatchVersion{info.mPatchVersion},
                              mName{info.mName}, mWidth{width}, mHeight{height} {
    jun::Logger::init();
    jun::Logger::trace("Constructing JuniperBase");
    mJWindow.init(mWidth, mHeight, mName);
    mJVkInstance.init(info);
    jun::Logger::trace("JuniperBase constructed");
}

jun::JuniperBase::~JuniperBase() {
    cleanup();
}

void jun::JuniperBase::run() {
    mainLoop();
}

void jun::JuniperBase::cleanup() {
    jun::Logger::trace("Cleaning up JuniperBase");
    mJVkInstance.cleanup();
    mJWindow.cleanup();
    jun::Logger::trace("JuniperBase cleaned up");
}

void jun::JuniperBase::mainLoop() {
    while (!mJWindow.shouldClose()) {
        glfwPollEvents();
    }
}