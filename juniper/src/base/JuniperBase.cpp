#include "JuniperBase.h"

#include <memory>
#include <string>

#include "Core.h"
#include "AppInfo.h"
#include "JuniperContext.h"
#include "JuniperDevice.h"
#include "JuniperSurface.h"
#include "JuniperSwapChain.h"
#include "JuniperVkInstance.h"
#include "JuniperWindow.h"

jun::JuniperBase::JuniperBase(
    const AppInfo& appInfo,
    int windowWidth,
    int windowHeight
) :
    mName(appInfo.mName),
    mMajorVersion(appInfo.mMajorVersion),
    mMinorVersion(appInfo.mMinorVersion),
    mPatchVersion(appInfo.mPatchVersion),
    mWindowWidth(windowWidth),
    mWindowHeight(windowHeight),
    mJContext(),
    mJWindow(mWindowWidth, mWindowHeight, mName, mJContext),
    mJVkInstance(appInfo, mJContext),
    mJSurface(mJContext),
    mJDevice(appInfo, mJContext),
    mJSwapChain(mJContext),
    mJPipeline(mJContext),
    mJFrameBuffer(mJContext),
    mJCommandBuffer(mJContext)
{
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

    // Clean these up in the reverse order of creation. Can't use destructors for
    // this because they will be destructed in the order they were created, which
    // we do not want.
    mJCommandBuffer.cleanup();
    mJFrameBuffer.cleanup();
    mJPipeline.cleanup();
    mJSwapChain.cleanup();
    mJDevice.cleanup();
    mJSurface.cleanup();
    mJVkInstance.cleanup();
    mJWindow.cleanup();
}

void jun::JuniperBase::mainLoop() {
    /**
     * @todo Actual game loop with constant physics rate and variable
     * rendering frame rate
     */
    while (!mJWindow.shouldClose()) {
        glfwPollEvents();
    }
}