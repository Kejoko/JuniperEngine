#include "JuniperBase.h"

#include "Core.h"
#include "AppInfo.h"
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
                              mName{info.mName}, mWidth{width},
                              mHeight{height},
                              mpInstance{std::make_shared<VkInstance>()},
                              mppWindow{std::make_shared<GLFWwindow*>()},
                              mpSurface{std::make_shared<VkSurfaceKHR>()},
                            //   mpPhysicalDevice{std::make_shared<VkPhysicalDevice>(VK_NULL_HANDLE)},
                              mpPhysicalDevice{std::make_shared<VkPhysicalDevice>()},
                              mpDevice{std::make_shared<VkDevice>()},
                              mpGraphicsQueue{std::make_shared<VkQueue>()},
                              mpPresentQueue{std::make_shared<VkQueue>()},
                              mJWindow{mWidth, mHeight, mName, mppWindow},
                              mJVkInstance{info, mpInstance},
                              mJSurface{mpInstance, mppWindow, mpSurface},
                            //   mJDevice{info, mpInstance, mppWindow, mpSurface},
                              mJDevice{info, mpInstance, mppWindow, mpSurface, mpPhysicalDevice, mpDevice, mpGraphicsQueue, mpPresentQueue},
                              mJSwapChain{mppWindow, mpSurface, mpPhysicalDevice, mpDevice} {
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