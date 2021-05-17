#pragma once

#include "Core.h"
#include "JuniperContext.h"

namespace jun {
    class JuniperPipeline;
}

class jun::JuniperPipeline {
    public:
        JuniperPipeline(const JuniperContext& context);
        ~JuniperPipeline() = default;

        void cleanup();
    private:
        void createGraphicsPipeline();
};