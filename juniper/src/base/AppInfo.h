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

    const std::vector<const char*> mValidationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char*> mDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    #if defined(BUILD_DEBUG)
    const bool mEnableValidationLayers = true;
    #else
    const bool mEnableValidationLayers = false;
    #endif
};