#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Pipeline: public Utility::Unique_handle<VkPipeline> {
    public:
	Pipeline() = default;

	Pipeline(
	    class Logical_device& _logical_device, 
	    class Shader_module& _vertex_shader, class Shader_module& _fragment_shader, 
	    std::uint32_t _viewport_width, std::uint32_t _viewport_height,
	    VkFormat _colour_attachment_format,
	    class Render_pass& _render_pass,
	    std::uint32_t _subpass_index
	);

	~Pipeline();

	Pipeline(Pipeline&&) = default;
	Pipeline& operator=(Pipeline&&) = default;

    private:
	class Layout: public Utility::Unique_handle<VkPipelineLayout> {
	public:
	    Layout() = default;
	    Layout(class Logical_device& _logical_device);
	} layout;

    };
}

