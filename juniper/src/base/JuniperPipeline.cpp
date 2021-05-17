#include "JuniperPipeline.h"

#include "Core.h"
#include "JuniperContext.h"
#include "util/FileIO.h"

#include <memory>
#include <vector>

jun::JuniperPipeline::JuniperPipeline(const JuniperContext& context) :
                                      mpDevice{context.mpDevice} {
    createGraphicsPipeline();

    jun::Logger::trace("JuniperGraphicsPipeline initialized");
}

void jun::JuniperPipeline::cleanup() {
    jun::Logger::trace("Cleaning up JuniperGraphicsPipeline");

}

void jun::JuniperPipeline::createGraphicsPipeline() {
    std::vector<char> vertShaderCode = readCompiledShader("shaders/shader.vert.spv");
    jun::Logger::debug("Vert shader file size: " + std::to_string(vertShaderCode.size()) + " bytes");
    std::vector<char> fragShaderCode = readCompiledShader("shaders/shader.frag.spv");
    jun::Logger::debug("Frag shader file size: " + std::to_string(fragShaderCode.size()) + " bytes");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
    
    /*
    Put this stuff in cleanup??
    Make the shader modules member variables?
    */
    vkDestroyShaderModule(*mpDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(*mpDevice, vertShaderModule, nullptr);
}


VkShaderModule jun::JuniperPipeline::createShaderModule(const std::vector<char>& byteCode) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = byteCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(byteCode.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(*mpDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        jun::Logger::critical("Failed to create shader module");
        throw std::runtime_error("Failed to create shader module");
    }

    return shaderModule;
}