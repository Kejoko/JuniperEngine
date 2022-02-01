#include "JuniperSurface.h"

#include <memory>

#include "Core.h"
#include "JuniperContext.h"

jun::JuniperSurface::JuniperSurface(const JuniperContext& context) :
    mpInstance(context.mpInstance),
    mppWindow(context.mppWindow),
    mpSurface(context.mpSurface)
{
    createSurface();

    jun::Logger::trace("JuniperSurface initialized");
}

void jun::JuniperSurface::cleanup() {
    vkDestroySurfaceKHR(*mpInstance, *mpSurface, nullptr);
}

void jun::JuniperSurface::createSurface() {
    if (glfwCreateWindowSurface(*mpInstance, *mppWindow, nullptr, mpSurface.get()) != VK_SUCCESS) {
        jun::Logger::critical("Failed to create window surface");
        throw std::runtime_error("Failed to create window surface");
    }
}