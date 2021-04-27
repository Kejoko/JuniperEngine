#include "Logger.h"

#include <iostream>

jun::Logger::Logger() {
    val = 42;
}

jun::Logger& jun::Logger::getInstance() {
    static Logger instance;
    return instance;
}

void jun::Logger::log() {
    std::cout << "Logging: " << val << '\n';
}