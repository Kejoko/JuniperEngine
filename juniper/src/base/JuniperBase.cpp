#include "JuniperBase.h"

#include "Core.h"
#include "AppInfo.h"
#include "JuniperDevice.h"
#include "JuniperVkInstance.h"
#include "JuniperWindow.h"

#include <memory>
#include <string>

jun::JuniperBase::JuniperBase(const AppInfo& info, int width, int height) :
                              mMajorVersion{info.mMajorVersion},
                              mMinorVersion{info.mMinorVersion},
                              mPatchVersion{info.mPatchVersion},
                              mName{info.mName}, mWidth{width},
                              mHeight{height},
                              mpInstance{std::make_shared<VkInstance>()},
                              mJWindow{mWidth, mHeight, mName},
                              mJVkInstance{info, mpInstance},
                              mJDevice{mpInstance} {
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

    mJDevice.cleanup();
    mJVkInstance.cleanup();
    mJWindow.cleanup();
}

void jun::JuniperBase::mainLoop() {
    while (!mJWindow.shouldClose()) {
        glfwPollEvents();
    }
}