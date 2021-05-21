#include "JuniperCommandBuffer.h"

#include "Core.h"
#include "JuniperContext.h"
#include "JuniperDevice.h"

#include <memory>
#include <vector>

jun::JuniperCommandBuffer::JuniperCommandBuffer(const JuniperContext& context) :
                                                mpSurface{context.mpSurface},
                                                mpPhysicalDevice{context.mpPhysicalDevice},
                                                mpDevice{context.mpDevice},
                                                mpCommandPool{context.mpCommandPool} {
    createCommandPool();
    createCommandBuffers();
    jun::Logger::trace("JuniperCommandBuffer initialized");
}

void jun::JuniperCommandBuffer::cleanup() {
    jun::Logger::trace("Cleaning up JuniperCommandBuffer");

    vkDestroyCommandPool(*mpDevice, *mpCommandPool, nullptr);
}

void jun::JuniperCommandBuffer::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices;
    queueFamilyIndices.findQueueFamilies(*mpPhysicalDevice, *mpSurface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.mGraphicsFamily.value();
    poolInfo.flags = 0;

    if (vkCreateCommandPool(*mpDevice, &poolInfo, nullptr, mpCommandPool.get()) != VK_SUCCESS) {
        jun::Logger::critical("Failed to create command pool");
        throw std::runtime_error("Failed to create command pool");
    }
}

void jun::JuniperCommandBuffer::createCommandBuffers() {
    
}