#include "synchronization.h"
#include "util.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_setup.h"

VkSemaphore createSemaphore(const VulkanState *const s) {
    VkSemaphoreCreateInfo semaphore_info = {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,};

    VkSemaphore semaphore;

    handleVkError("Failed to create semaphore", 
        vkCreateSemaphore(s->device, &semaphore_info, NULL, &semaphore)
    );

    return semaphore;
}

VkFence createFence(const VulkanState *const s) {
    VkFenceCreateInfo fenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    VkFence fence;

    handleVkError("Failed to create fence", 
        vkCreateFence(s->device, &fenceInfo, NULL, &fence)
    );

    return fence;
}

