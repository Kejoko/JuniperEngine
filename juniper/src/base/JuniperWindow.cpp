#include "JuniperWindow.h"

#include "Core.h"
#include "JuniperContext.h"

#include <string>

jun::JuniperWindow::JuniperWindow(int width, int height, const std::string& name, const JuniperContext& context) :
                                  mWidth{width},
                                  mHeight{height},
                                  mName{name},
                                  mppWindow{context.mppWindow} {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    *mppWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), nullptr, nullptr);
    
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