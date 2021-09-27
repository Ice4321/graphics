#ifndef INCLUDED_GRAPHICS_PIPELINE_HPP
#define INCLUDED_GRAPHICS_PIPELINE_HPP

namespace Graphics { class Swap_chain; }
#include"graphics/shader/module.hpp"

namespace Graphics {
    class Pipeline {
    public:
	// TODO: make this variadic
	Pipeline(class Logical_device& _logical_device, Shader_module& _vertex_shader, Shader_module& _fragment_shader, Swap_chain& _swap_chain);
	
	operator VkPipeline& () noexcept;

	VkRenderPass& get_render_pass() noexcept;

	~Pipeline();

	Pipeline(Pipeline const&) = delete;
	Pipeline& operator=(Pipeline const&) = delete;

    private:
	class Logical_device* logical_device;
	VkPipelineLayout layout;
	VkRenderPass render_pass;
	VkPipeline pipeline;
    };
}

#endif
