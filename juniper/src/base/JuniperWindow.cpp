#include "JuniperWindow.h"

#include "Core.h"

#include <string>

void jun::JuniperWindow::init(int width, int height, std::string name) {
    jun::Logger::trace("Initializing JuniperWindow");

    mWidth = width;
    mHeight = height;
    mName = name;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr, nullptr);
    
    jun::Logger::trace("JuniperWindow initialized");
}

void jun::JuniperWindow::cleanup() {
    jun::Logger::trace("Cleaning up JuniperWindow");
    glfwDestroyWindow(mWindow);
    glfwTerminate();
    jun::Logger::trace("JuniperWindow cleaned up");
}

bool jun::JuniperWindow::shouldClose() {
    return glfwWindowShouldClose(mWindow);
}