#include"graphics/pipeline.hpp"

Graphics::Pipeline::Pipeline(Shader_module& _vertex_shader, Shader_module& _fragment_shader) {
    // TODO: use pSpecializationInfo
    VkPipelineShaderStageCreateInfo vertex_shader_stage_create_info{
	.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.stage = VK_SHADER_STAGE_VERTEX_BIT,
	.module = _vertex_shader,
	.pName = "main",
	.pSpecializationInfo = nullptr
    };

    VkPipelineShaderStageCreateInfo fragment_shader_stage_create_info{
	.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
	.module = _fragment_shader,
	.pName = "main",
	.pSpecializationInfo = nullptr
    };
}

Graphics::Pipeline::~Pipeline() {

}
