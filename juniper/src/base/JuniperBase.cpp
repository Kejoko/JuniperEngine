#include "JuniperBase.h"

#include "JuniperWindow.h"

#include <string>

jun::JuniperBase::JuniperBase(int width, int height, std::string name) : mWidth{width}, mHeight{height}, mName{name} {
    jun::Logger::init();
    jun::Logger::trace("Constructing JuniperBase");
    mJWindow.init(mWidth, mHeight, mName);
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
    mJWindow.cleanup();
    jun::Logger::trace("JuniperBase cleaned up");
}

void jun::JuniperBase::mainLoop() {
    while (!mJWindow.shouldClose()) {
        glfwPollEvents();
    }
}