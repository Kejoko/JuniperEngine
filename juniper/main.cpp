#include "base/JuniperBase.h"
#include "logger/Logger.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    jun::JuniperBase base(0, 0, 1, 800, 600, "Juniper Engine");

    try {
        base.run();
    } catch (const std::exception& e) {
        std::string excMsg(e.what());
        jun::Logger::critical("Exception raised running base in main:\n" + excMsg);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
