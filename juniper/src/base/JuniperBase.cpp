#include "JuniperBase.h"

#include "Core.h"
#include "JuniperVkInstance.h"
#include "JuniperWindow.h"

#include <string>

jun::JuniperBase::JuniperBase(int major, int minor, int patch, int width, int height, std::string name) :
                              mMajorVersion{major}, mMinorVersion{minor}, mPatchVersion{patch},
                              mWidth{width}, mHeight{height}, mName{name} {
    jun::Logger::init();
    jun::Logger::trace("Constructing JuniperBase");
    mJWindow.init(mWidth, mHeight, mName);
    mJVkInstance.init(mMajorVersion, mMinorVersion, mPatchVersion, mName);
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