#include <stdint.h>

#include "util.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_setup.h"

#include "vulkan_setup/pipeline.h"
#include "math_types.h"

align(4)
const unsigned char VERTEX_SHADER[] =
#include "shader.vert.h"

align(4)
const unsigned char FRAGMENT_SHADER[] =
#include "shader.frag.h"

static VkShaderModule createShaderModule(const VkDevice device, const unsigned char *const bytecode, const uint32_t code_length);

VkPipeline createGraphicsPipeline(const VulkanState *const s, VkPipelineLayout *const o_pipeline_layout) {
    VkShaderModule vertex_shader_module = createShaderModule(s->device, VERTEX_SHADER, ARRAY_LENGTH(VERTEX_SHADER));
    VkShaderModule fragment_shader_module = createShaderModule(s->device, FRAGMENT_SHADER, ARRAY_LENGTH(FRAGMENT_SHADER));

    // Shader stages //
    VkPipelineShaderStageCreateInfo vert_shader_stage_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertex_shader_module,
        .pName = "main",
    };

    VkPipelineShaderStageCreateInfo frag_shader_stage_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragment_shader_module,
        .pName = "main",
    };

    VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info, frag_shader_stage_info};

    // Vertex info //
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,

        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = &Vertex_BINDING_DESCRIPTION,

        .vertexAttributeDescriptionCount = ARRAY_LENGTH(Vertex_ATTRIBUTE_DESCRIPTIONS),
        .pVertexAttributeDescriptions = &Vertex_ATTRIBUTE_DESCRIPTIONS[0],
    };

    // Dynamic states //
    VkDynamicState dynamic_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = ARRAY_LENGTH(dynamic_states),
        .pDynamicStates = dynamic_states,
    };

    // Input Assembly //
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };

    // Viewport & Scissor //
    VkPipelineViewportStateCreateInfo viewportState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .scissorCount = 1,
    };

    // Rasterizer //
    VkPipelineRasterizationStateCreateInfo rasterizer = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .lineWidth = 1.0f,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f, // Optional
        .depthBiasClamp = 0.0f, // Optional
        .depthBiasSlopeFactor = 0.0f, // Optional
    };

    // Disable multisampling //
    VkPipelineMultisampleStateCreateInfo multisampling = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .sampleShadingEnable = VK_FALSE,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .minSampleShading = 1.0f, // Optional
        .pSampleMask = NULL, // Optional
        .alphaToCoverageEnable = VK_FALSE, // Optional
        .alphaToOneEnable = VK_FALSE, // Optional
    };

    // Color blending //
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        .blendEnable = VK_TRUE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
    };

    VkPipelineColorBlendStateCreateInfo colorBlending = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY, // Optional
        .attachmentCount = 1,
        .pAttachments = &colorBlendAttachment,
        .blendConstants[0] = 0.0f, // Optional
        .blendConstants[1] = 0.0f, // Optional
        .blendConstants[2] = 0.0f, // Optional
        .blendConstants[3] = 0.0f, // Optional
    };

    // Pipeline layout //
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1, // Optional
        .pSetLayouts = &s->descriptor_set_layout, // Optional
        .pushConstantRangeCount = 0, // Optional
        .pPushConstantRanges = NULL, // Optional
    };

    handleVkError("Failed to create pipeline layout", 
        vkCreatePipelineLayout(s->device, &pipelineLayoutInfo, NULL, o_pipeline_layout)
    );

    VkGraphicsPipelineCreateInfo pipelineInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = ARRAY_LENGTH(shader_stages),
        .pStages = shader_stages,
        .pVertexInputState = &vertexInputInfo,
        .pInputAssemblyState = &inputAssembly,
        .pViewportState = &viewportState,
        .pRasterizationState = &rasterizer,
        .pMultisampleState = &multisampling,
        .pDepthStencilState = NULL, // Optional
        .pColorBlendState = &colorBlending,
        .pDynamicState = &dynamicState,
        .layout = *o_pipeline_layout,
        .subpass = 0,
        .renderPass = s->render_pass,

        .basePipelineHandle = VK_NULL_HANDLE, // Optional
        .basePipelineIndex = -1, // Optional
    };

    VkPipeline graphicsPipeline;
    handleVkError("Failed to create graphics pipeline", 
        vkCreateGraphicsPipelines(s->device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &graphicsPipeline)
    );

    vkDestroyShaderModule(s->device, fragment_shader_module, NULL);
    vkDestroyShaderModule(s->device, vertex_shader_module, NULL);

    return graphicsPipeline;
}

static VkShaderModule createShaderModule(const VkDevice device, const unsigned char *const bytecode, const uint32_t code_length) {
    const uint32_t *const shader = (uint32_t *) bytecode;

    VkShaderModuleCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code_length,
        .pCode = shader,
    };

    VkShaderModule shader_module;
    handleVkError("Failed to create shader module", 
        vkCreateShaderModule(device, &create_info, NULL, &shader_module)
    );

    return shader_module;
}
