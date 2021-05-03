#include "JuniperSurface.h"

#include "Core.h"

#include <memory>

jun::JuniperSurface::JuniperSurface(std::shared_ptr<VkInstance> pInstance, std::shared_ptr<GLFWwindow*> ppWindow, std::shared_ptr<VkSurfaceKHR> pSurface) :
                                    mpInstance{pInstance}, mppWindow{ppWindow}, mpSurface{pSurface} {
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