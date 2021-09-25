#pragma once

#include <vulkan/vulkan.h>
#include "utility/unique_handle.hpp"

namespace Graphics {
    class Shader_module: public Utility::Unique_handle<VkShaderModule> {
    public:
	// The binary can be destroyed immediately after creating the shader module
	Shader_module(class Shader_binary const& _binary, class Logical_device* _logical_device);

    private:
	class Logical_device* logical_device;
    };
};

