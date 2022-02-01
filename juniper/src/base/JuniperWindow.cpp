#include "JuniperWindow.h"

#include <string>

#include "Core.h"
#include "JuniperContext.h"

jun::JuniperWindow::JuniperWindow(
    int windowWidth,
    int windowHeight,
    const std::string& name,
    const JuniperContext& context
) :
    mWindowWidth(windowWidth),
    mWindowHeight(windowHeight),
    mName(name),
    mppWindow(context.mppWindow) 
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    *mppWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, mName.c_str(), nullptr, nullptr);
    
    jun::Logger::trace("JuniperWindow initialized");
}

void jun::JuniperWindow::cleanup() {
    jun::Logger::trace("Cleaning up JuniperWindow");

    glfwDestroyWindow(*mppWindow);
    glfwTerminate();
}

bool jun::JuniperWindow::shouldClose() {
    return glfwWindowShouldClose(*mppWindow);
}