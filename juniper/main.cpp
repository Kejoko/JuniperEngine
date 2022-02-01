#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "base/Core.h"
#include "base/AppInfo.h"
#include "base/JuniperBase.h"
#include "logger/Logger.h"

int main() {
    jun::Logger::init();

    jun::AppInfo appInfo;
    appInfo.mName = "Our Lovely Game";
    appInfo.mMajorVersion = 0;
    appInfo.mMinorVersion = 0;
    appInfo.mPatchVersion = 1;

    jun::JuniperBase base(appInfo, 800, 600);

    try {
        base.run();
    } catch (const std::exception& e) {
        std::string exceptionMessage(e.what());
        jun::Logger::critical("Exception raised running base in main:\n" + exceptionMessage);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
