#pragma once

#include "Core.h"

namespace jun {
    struct AppInfo;
}

struct jun::AppInfo {
    std::string mName;
    int mMajorVersion;
    int mMinorVersion;
    int mPatchVersion;

    std::vector<const char*> mValidationLayers;
    #if defined(BUILD_DEBUG)
    const bool mEnableValidationLayers = true;
    #else
    const bool mEnableValidationLayers = false;
    #endif
};