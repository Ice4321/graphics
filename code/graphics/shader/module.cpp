#include "graphics/shader/module.hpp"
#include "graphics/shader/binary.hpp"
#include "graphics/device/logical.hpp"
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Shader_module::Shader_module(Logical_device& _logical_device, Shader_binary const& _binary):
    kind(_binary.get_kind())
{
    VkShaderModuleCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.codeSize = _binary.get_data().size(),
	.pCode = reinterpret_cast<std::uint32_t const*>(_binary.get_data().data())
    };
    
    Handle shader_module;
    VULKAN_ASSERT(vkCreateShaderModule(_logical_device, &create_info, nullptr, &shader_module)); 
    Unique_handle::operator=({shader_module, [&_logical_device](Handle _shader_module) { vkDestroyShaderModule(_logical_device, _shader_module, nullptr); }});

    shader_stage_creation_info = {
	.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.stage = kind,
	.module = get_handle(),
	.pName = "main",
	.pSpecializationInfo = nullptr
    };
}

Graphics::Shader_kind const& Graphics::Shader_module::get_kind() const noexcept {
    return kind;
}

VkPipelineShaderStageCreateInfo const& Graphics::Shader_module::get_shader_stage_creation_info() const noexcept {
    return shader_stage_creation_info;
}
