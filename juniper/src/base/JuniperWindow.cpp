#include "JuniperWindow.h"

#include "Core.h"

#include <string>

jun::JuniperWindow::JuniperWindow(int width, int height, const std::string& name) :
                                  mWidth{width}, mHeight{height}, mName{name} {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    mpWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr, nullptr);
    
    jun::Logger::trace("JuniperWindow initialized");
}

void jun::JuniperWindow::cleanup() {
    jun::Logger::trace("Cleaning up JuniperWindow");

    glfwDestroyWindow(mpWindow);
    glfwTerminate();
}

bool jun::JuniperWindow::shouldClose() {
    return glfwWindowShouldClose(mpWindow);
}