#include "graphics/shader/module.hpp"
#include "graphics/shader/binary.hpp"
#include "graphics/devices/logical_device.hpp"
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Shader_module::Shader_module(Shader_binary const& _binary, Logical_device* _logical_device):
    logical_device(_logical_device)
{
    VkShaderModuleCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.codeSize = _binary.get_data().size(),
	.pCode = reinterpret_cast<std::uint32_t const*>(_binary.get_data().data())
    };
    
    VkShaderModule shader_module;
    VULKAN_ASSERT(vkCreateShaderModule(*logical_device, &create_info, nullptr, &shader_module)); 
    Unique_handle::operator=({shader_module, [_logical_device](Handle _shader_module) { vkDestroyShaderModule(*_logical_device, _shader_module, nullptr); }});
}