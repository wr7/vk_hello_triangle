// #include "vulkan_setup/pipeline.h"
#include "util.h"
#include "vulkan_setup.h"
#include <stdint.h>

// Mildly janky way to ensure that VERTEX_SHADER and FRAGMENT_SHADER are 32-bit aligned //
__attribute__( ( aligned ( 4 ) ) )
#include "vertex_shader.h"
__attribute__( ( aligned ( 4 ) ) )
#include "fragment_shader.h"

static VkShaderModule createShaderModule(const VkDevice device, const unsigned char *const bytecode, const uint32_t code_length);

void createGraphicsPipeline(const VulkanState *const s) {
    VkShaderModule vertex_shader_module = createShaderModule(s->device, VERTEX_SHADER, ARRAY_LENGTH(VERTEX_SHADER));
    VkShaderModule fragment_shader_module = createShaderModule(s->device, FRAGMENT_SHADER, ARRAY_LENGTH(FRAGMENT_SHADER));

    vkDestroyShaderModule(s->device, fragment_shader_module, NULL);
    vkDestroyShaderModule(s->device, vertex_shader_module, NULL);
}

static VkShaderModule createShaderModule(const VkDevice device, const unsigned char *const bytecode, const uint32_t code_length) {
    const uint32_t *const shader = (uint32_t *) bytecode;

    VkShaderModuleCreateInfo create_info = {0};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = code_length;
    create_info.pCode = shader;

    VkShaderModule shader_module;
    handleVkError("Failed to create shader module", 
        vkCreateShaderModule(device, &create_info, NULL, &shader_module)
    );

    return shader_module;
}
