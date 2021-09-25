#include"graphics/shader_module.hpp"
#include"graphics/utility/vulkan_assert.hpp"

Graphics::Shader_module::Shader_module(Shader_binary const& _binary, Logical_device& _logical_device):
    logical_device(&_logical_device)
{
    VkShaderModuleCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.codeSize = _binary.get_data().size(),
	.pCode = reinterpret_cast<std::uint32_t const*>(_binary.get_data().data())
    };

    VULKAN_ASSERT(vkCreateShaderModule(*logical_device, &create_info, nullptr, &shader_module)); 

}

Graphics::Shader_module::~Shader_module() {
    vkDestroyShaderModule(*logical_device, shader_module, nullptr);
}


Graphics::Shader_module::operator VkShaderModule& () noexcept {
    return shader_module;
}
