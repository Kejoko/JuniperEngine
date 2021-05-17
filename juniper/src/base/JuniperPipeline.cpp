#include "JuniperPipeline.h"

#include "Core.h"
#include "JuniperContext.h"
#include "util/FileIO.h"

#include <vector>

jun::JuniperPipeline::JuniperPipeline(const JuniperContext& context) {
    createGraphicsPipeline();

    jun::Logger::trace("JuniperGraphicsPipeline initialized");
}

void jun::JuniperPipeline::cleanup() {
    jun::Logger::trace("Cleaning up JuniperGraphicsPipeline");

}

void jun::JuniperPipeline::createGraphicsPipeline() {
    std::vector<char> vertShaderCode = readCompiledShader("juniper/shaders/vert.spv");
    jun::Logger::debug("Vert shader file size: " + std::to_string(vertShaderCode.size()) + " bytes");
    std::vector<char> fragShaderCode = readCompiledShader("juniper/shaders/frag.spv");
    jun::Logger::debug("Frag shader file size: " + std::to_string(fragShaderCode.size()) + " bytes");
}