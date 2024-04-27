#include "descriptor_sets.h"
#include "math_types.h"
#include "util.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_setup.h"
#include <stddef.h>

VkDescriptorSetLayout createDescriptorSetLayout(const VulkanState *const s) {
    VkDescriptorSetLayoutBinding uboLayoutBinding = {
        .binding = 0,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = 1,
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings = &uboLayoutBinding,
    };

    VkDescriptorSetLayout layout;
    handleVkError("Failed to create descriptor set layout",
        vkCreateDescriptorSetLayout(s->device, &layoutInfo, NULL, &layout)
    );

    return layout;
}

VkDescriptorPool createDescriptorPool(const VulkanState *const s) {
    VkDescriptorPoolSize pool_size = {
        .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount = FRAMES_IN_FLIGHT,
    };

    VkDescriptorPoolCreateInfo pool_info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .poolSizeCount = 1,
        .pPoolSizes = &pool_size,
        .maxSets = FRAMES_IN_FLIGHT,
    };

    VkDescriptorPool pool;

    handleVkError("Failed to create descriptor pool",
        vkCreateDescriptorPool(s->device, &pool_info, NULL, &pool)
    );

    return pool;
}

void createDescriptorSets(const VulkanState *const s, VkDescriptorSet (*const o_sets)[FRAMES_IN_FLIGHT]) {
    VkDescriptorSetLayout layouts[ARRAY_LENGTH(*o_sets)];

    for(size_t i = 0; i < ARRAY_LENGTH(layouts); i++) {
        layouts[i] = s->descriptor_set_layout;
    }

    VkDescriptorSetAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = s->descriptor_pool,
        .descriptorSetCount = ARRAY_LENGTH(layouts),
        .pSetLayouts = &layouts[0],
    };

    handleVkError("Failed to allocate descriptor sets",
        vkAllocateDescriptorSets(s->device, &alloc_info, &(*o_sets)[0])
    );

    const VkDeviceSize MIN_UBO_ALIGNMENT = s->gpu_properties.limits.minUniformBufferOffsetAlignment;
    const VkDeviceSize UBO_OFFSET = cdiv(sizeof(UniformBufferObject), MIN_UBO_ALIGNMENT) * MIN_UBO_ALIGNMENT;

    for(VkDeviceSize i = 0; i < ARRAY_LENGTH(*o_sets); i++) {
        VkDescriptorBufferInfo buffer_info = {
            .buffer = s->uniform_buffer,
            .offset = i * UBO_OFFSET,
            .range = sizeof(UniformBufferObject),
        };

        VkWriteDescriptorSet descriptor_write = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .dstSet = (*o_sets)[i],
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            .pBufferInfo = &buffer_info,
            .pImageInfo = NULL, // Optional
            .pTexelBufferView = NULL, // Optional
        };

        vkUpdateDescriptorSets(s->device, 1, &descriptor_write, 0, NULL);
   }
}
