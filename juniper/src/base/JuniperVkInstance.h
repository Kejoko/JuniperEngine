#pragma once

#include "Core.h"
#include "AppInfo.h"

namespace jun {
    class JuniperVkInstance;
}

class jun::JuniperVkInstance {
    public:
        JuniperVkInstance() = default;
        ~JuniperVkInstance() = default;

        JuniperVkInstance(const JuniperVkInstance&) = delete;
        JuniperVkInstance& operator=(const JuniperVkInstance&) = delete;

        void init(const AppInfo& info);
        void cleanup();

    private:
        int mMajorVersion;
        int mMinorVersion;
        int mPatchVersion;
        std::string mName;
        VkInstance mInstance;

        void createInstance();
};