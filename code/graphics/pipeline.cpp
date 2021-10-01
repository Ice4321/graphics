#include"graphics/pipeline.hpp"
#include"graphics/swap_chain.hpp"
#include"graphics/utility/vulkan_assert.hpp"
#include"graphics/device/logical.hpp"

Graphics::Pipeline::Pipeline(Logical_device& _logical_device, Shader_module& _vertex_shader, Shader_module& _fragment_shader, Swap_chain& _swap_chain):
    logical_device(&_logical_device)
{
    // TODO: use pSpecializationInfo
    VkPipelineShaderStageCreateInfo shader_stages_create_info[] = {
	_vertex_shader.get_shader_stage_creation_info(),
	_fragment_shader.get_shader_stage_creation_info()
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{
	.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.vertexBindingDescriptionCount = 0,
	.pVertexBindingDescriptions = nullptr,
	.vertexAttributeDescriptionCount = 0,
	.pVertexAttributeDescriptions = nullptr
    };

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info{
	.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	.primitiveRestartEnable = VK_FALSE
    };

    VkViewport viewports[] = {{
	.x = 0,
	.y = 0,
	.width = (float)_swap_chain.get_image_extent().width,
	.height = (float)_swap_chain.get_image_extent().height,
	.minDepth = 0.0f,
	.maxDepth = 1.0f
    }};

    VkRect2D scissors[] = {{
	.offset = { .x = 0, .y = 0 },
	.extent = _swap_chain.get_image_extent()
    }};

    VkPipelineViewportStateCreateInfo viewport_state_create_info{
	.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.viewportCount = 1,
	.pViewports = viewports,
	.scissorCount = 1,
	.pScissors = scissors
    };
    
    VkPipelineRasterizationStateCreateInfo rasterisation_state_create_info{
	.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.depthClampEnable = VK_FALSE,
	.rasterizerDiscardEnable = VK_FALSE,
	.polygonMode = VK_POLYGON_MODE_FILL,
	.cullMode = VK_CULL_MODE_NONE,
	.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
	.depthBiasEnable = VK_FALSE,
	.depthBiasConstantFactor = {},
	.depthBiasClamp = {},
	.depthBiasSlopeFactor = {},
	.lineWidth = 1.0f
    };

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info{
	.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	.sampleShadingEnable = VK_FALSE,
	.minSampleShading = {},
	.pSampleMask = {},
	.alphaToCoverageEnable = {},
	.alphaToOneEnable = {}
    };

    VkPipelineColorBlendAttachmentState colour_blend_attachment_states[] = {{
	.blendEnable = VK_FALSE,
	.srcColorBlendFactor = {},
	.dstColorBlendFactor = {},
	.colorBlendOp = {},
	.srcAlphaBlendFactor = {},
	.dstAlphaBlendFactor = {},
	.alphaBlendOp = {},
	.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    }};

    VkPipelineColorBlendStateCreateInfo colour_blend_state_create_info{
	.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.logicOpEnable = VK_FALSE,
	.logicOp = {},
	.attachmentCount = 1,
	.pAttachments = colour_blend_attachment_states,
	.blendConstants = {}
    };

    VkPipelineLayoutCreateInfo pipeline_layout_create_info{
	.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.setLayoutCount = 0,
	.pSetLayouts = nullptr,
	.pushConstantRangeCount = 0,
	.pPushConstantRanges = nullptr
    };

    VULKAN_ASSERT(vkCreatePipelineLayout(*logical_device, &pipeline_layout_create_info, nullptr, &layout)); 

    VkAttachmentDescription colour_attachment_descriptions[] = {{
	.flags = 0,
	.format = _swap_chain.get_image_format(),
	.samples = VK_SAMPLE_COUNT_1_BIT,
	.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
	.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
	.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
	.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
	.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    }};

    VkAttachmentReference colour_attachment_references[] = {{
	.attachment = 0,
	.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    }};

    // Subpasses are subsequent rendering operations that depend on the contents of framebuffers in previous passes, 
    // for example a sequence of post-processing effects that are applied one after another.
    VkSubpassDescription subpass_descriptions[] = {{
	.flags = 0,
	.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
	.inputAttachmentCount = 0,
	.pInputAttachments = nullptr,
	.colorAttachmentCount = 1,
	.pColorAttachments = colour_attachment_references,
	.pResolveAttachments = 0,
	.pDepthStencilAttachment = nullptr,
	.preserveAttachmentCount = 0,
	.pPreserveAttachments= nullptr
    }};
    
    VkSubpassDependency subpass_dependencies[] = {{
	.srcSubpass = VK_SUBPASS_EXTERNAL,
	.dstSubpass = 0,
	.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
	.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
	.srcAccessMask = 0,
	.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
	.dependencyFlags = 0
    }};

    VkRenderPassCreateInfo render_pass_create_info{
	.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.attachmentCount = 1,
	.pAttachments = colour_attachment_descriptions,
	.subpassCount = 1,
	.pSubpasses = subpass_descriptions,
	.dependencyCount = 1,
	.pDependencies = subpass_dependencies
    };

    render_pass = {_logical_device, colour_attachment_descriptions, subpass_descriptions, subpass_dependencies};


    VkGraphicsPipelineCreateInfo pipeline_create_info[] = {{
	.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.stageCount = 2,
	.pStages = shader_stages_create_info,
	.pVertexInputState = &vertex_input_state_create_info,
	.pInputAssemblyState = &input_assembly_state_create_info,
	.pTessellationState = nullptr,
	.pViewportState = &viewport_state_create_info,
	.pRasterizationState = &rasterisation_state_create_info,
	.pMultisampleState = &multisample_state_create_info,
	.pDepthStencilState = nullptr,
	.pColorBlendState = &colour_blend_state_create_info,
	.pDynamicState = nullptr,
	.layout = layout,
	.renderPass = render_pass,
	.subpass = 0,
	.basePipelineHandle = VK_NULL_HANDLE,
	.basePipelineIndex = -1
    }};
    
    // TODO: Safe to use the address of a single object as if it was a 1-element array?
    VULKAN_ASSERT(vkCreateGraphicsPipelines(*logical_device, VK_NULL_HANDLE, 1, pipeline_create_info, nullptr, &pipeline));

}


Graphics::Pipeline::~Pipeline() {
    vkDestroyPipeline(*logical_device, pipeline, nullptr);
    render_pass = {}; // This will be removed later 
    vkDestroyPipelineLayout(*logical_device, layout, nullptr);
}

Graphics::Pipeline::operator VkPipeline& () noexcept {
    return pipeline;
}

VkRenderPass Graphics::Pipeline::get_render_pass() noexcept {
    return render_pass;
}
