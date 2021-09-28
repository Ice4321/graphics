#pragma once

#include <vulkan/vulkan.h>
#include "utility/unique_handle.hpp"
#include "graphics/shader/kind.hpp"

namespace Graphics {
    class Shader_module: public Utility::Unique_handle<VkShaderModule> {
    public:
	// The binary can be destroyed immediately after creating the shader module
	Shader_module(class Logical_device& _logical_device, class Shader_binary const& _binary);
	
	Shader_kind const& get_kind() const noexcept;

	VkPipelineShaderStageCreateInfo const& get_shader_stage_creation_info() const noexcept;

    private:
	Shader_kind kind;
	VkPipelineShaderStageCreateInfo shader_stage_creation_info;

    };
};

