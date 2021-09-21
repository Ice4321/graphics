#ifndef INCLUDED_GRAPHICS_PIPELINE_HPP
#define INCLUDED_GRAPHICS_PIPELINE_HPP

namespace Graphics { class Swap_chain; }
#include"graphics/shader_module.hpp"
#include"graphics/devices/logical_device.hpp"

namespace Graphics {
    class Pipeline {
    public:
	// TODO: make this variadic
	Pipeline(Shader_module& _vertex_shader, Shader_module& _fragment_shader, Swap_chain& _swap_chain, Logical_device& _logical_device);
	
	operator VkPipeline& () noexcept;

	VkRenderPass& get_render_pass() noexcept;

	~Pipeline();

	Pipeline(Pipeline const&) = delete;
	Pipeline& operator=(Pipeline const&) = delete;

    private:
	Logical_device* logical_device;
	VkPipelineLayout layout;
	VkRenderPass render_pass;
	VkPipeline pipeline;
    };
}

#endif
