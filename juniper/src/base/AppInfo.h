#pragma once

#include "Core.h"

#include <string>

namespace jun {
    struct AppInfo;
}

struct jun::AppInfo {
    std::string mName;
    int mMajorVersion;
    int mMinorVersion;
    int mPatchVersion;
};