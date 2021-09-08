#ifndef INCLUDED_GRAPHICS_PIPELINE_HPP
#define INCLUDED_GRAPHICS_PIPELINE_HPP

#include"graphics/shader_module.hpp"
#include"graphics/swap_chain.hpp"
#include"graphics/logical_device.hpp"

namespace Graphics {
    class Pipeline {
    public:
	// TODO: make this variadic
	Pipeline(Shader_module& _vertex_shader, Shader_module& _fragment_shader, Swap_chain& _swap_chain, Logical_device& _logical_device);

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
