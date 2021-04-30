#include "base/Core.h"
#include "base/AppInfo.h"
#include "base/JuniperBase.h"
#include "logger/Logger.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    jun::AppInfo appInfo;
    appInfo.mName = "Juniper Engine";
    appInfo.mMajorVersion = 0;
    appInfo.mMinorVersion = 0;
    appInfo.mPatchVersion = 1;
    jun::JuniperBase base(appInfo, 800, 600);

    try {
        base.run();
    } catch (const std::exception& e) {
        std::string excMsg(e.what());
        jun::Logger::critical("Exception raised running base in main:\n" + excMsg);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
