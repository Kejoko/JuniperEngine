#include "JuniperFrameBuffer.h"

#include "Core.h"
#include "JuniperContext.h"

#include <memory>
#include <vector>

jun::JuniperFrameBuffer::JuniperFrameBuffer(const JuniperContext& context) :
                                            mpDevice{context.mpDevice},
                                            mpSwapChainExtent{context.mpSwapChainExtent},
                                            mpSwapChainImageViews{context.mpSwapChainImageViews},
                                            mpRenderPass{context.mpRenderPass},
                                            mpSwapChainFramebuffers{context.mpSwapChainFramebuffers} {
    createFrameBuffers();
    jun::Logger::trace("JuniperFrameBuffer initialized");
}

void jun::JuniperFrameBuffer::cleanup() {
    jun::Logger::trace("Cleaning up JuniperFrameBuffer");

    for (VkFramebuffer framebuffer : *mpSwapChainFramebuffers) {
        vkDestroyFramebuffer(*mpDevice, framebuffer, nullptr);
    }
}

void jun::JuniperFrameBuffer::createFrameBuffers() {
    mpSwapChainFramebuffers->resize(mpSwapChainImageViews->size());

    for (size_t i = 0; i < mpSwapChainImageViews->size(); i++) {
        VkImageView attachments[] = {
            (*mpSwapChainImageViews)[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = *mpRenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = mpSwapChainExtent->width;
        framebufferInfo.height = mpSwapChainExtent->height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(*mpDevice, &framebufferInfo, nullptr, &(*mpSwapChainFramebuffers)[i]) != VK_SUCCESS) {
            jun::Logger::critical("Failed to create framebuffer");
            throw std::runtime_error("Failed to create framebuffer");
        }
    }
}