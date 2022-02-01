#include "JuniperCommandBuffer.h"

#include <memory>
#include <vector>

#include "Core.h"
#include "JuniperContext.h"
#include "JuniperDevice.h"

jun::JuniperCommandBuffer::JuniperCommandBuffer(const JuniperContext& context) :
    mpSurface(context.mpSurface),
    mpPhysicalDevice(context.mpPhysicalDevice),
    mpDevice(context.mpDevice),
    mpSwapChainExtent(context.mpSwapChainExtent),
    mpRenderPass(context.mpRenderPass),
    mpGraphicsPipeline(context.mpGraphicsPipeline),
    mpSwapChainFramebuffers(context.mpSwapChainFramebuffers),
    mpCommandPool(context.mpCommandPool),
    mpCommandBuffers(context.mpCommandBuffers)
{
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
    mpCommandBuffers->resize(mpSwapChainFramebuffers->size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = *mpCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) mpCommandBuffers->size();

    if (vkAllocateCommandBuffers(*mpDevice, &allocInfo, mpCommandBuffers->data()) != VK_SUCCESS) {
        jun::Logger::critical("Failed to allocate command buffers");
        throw std::runtime_error("Failed to allocate command buffers");
    }

    for (size_t i = 0; i < mpCommandBuffers->size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer((*mpCommandBuffers)[i], &beginInfo) != VK_SUCCESS) {
            jun::Logger::critical("Failed to begin recording command buffer");
            throw std::runtime_error("Failed to begin recording command buffer");
        }

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = *mpRenderPass;
        renderPassInfo.framebuffer = (*mpSwapChainFramebuffers)[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = *mpSwapChainExtent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass((*mpCommandBuffers)[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline((*mpCommandBuffers)[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *mpGraphicsPipeline);
        vkCmdDraw((*mpCommandBuffers)[i], 3, 1, 0, 0);
        vkCmdEndRenderPass((*mpCommandBuffers)[i]);

        if (vkEndCommandBuffer((*mpCommandBuffers)[i]) != VK_SUCCESS) {
            jun::Logger::critical("Failed to record command buffer");
            throw std::runtime_error("Failed to record command buffer");
        }
    }
}