#ifndef INCLUDED_GRAPHICS_SHADER_MODULE_HPP
#define INCLUDED_GRAPHICS_SHADER_MODULE_HPP

#include<vulkan/vulkan.h>
#include"graphics/shader_binary.hpp"
#include"graphics/devices/logical_device.hpp"

namespace Graphics {
    class Shader_module {
    public:
	// The binary can be destroyed immediately after creating the shader module
	Shader_module(Shader_binary const& _binary, Logical_device& _logical_device);

	operator VkShaderModule& () noexcept;

	~Shader_module();
	
	Shader_module(Shader_module const&) = delete;
	Shader_module& operator=(Shader_module const&) = delete;

    private:
	VkShaderModule shader_module;
	Logical_device* logical_device;
    };
};

#endif
