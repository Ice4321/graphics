#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>
#include "graphics/render_pass.hpp"

namespace Graphics {
    class Pipeline: public Utility::Unique_handle<VkPipeline> {
    public:
	Pipeline(
	    class Logical_device& _logical_device, 
	    class Shader_module& _vertex_shader, class Shader_module& _fragment_shader, 
	    class Swap_chain& _swap_chain
	);

	Render_pass& get_render_pass() noexcept;

	~Pipeline();

    private:
	Utility::Unique_handle<VkPipelineLayout> layout;
	Render_pass render_pass;

    };
}

