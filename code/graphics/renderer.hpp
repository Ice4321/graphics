#ifndef INCLUDED_GRAPHICS_RENDERER_HPP
#define INCLUDED_GRAPHICS_RENDERER_HPP

#include<vulkan/vulkan.h>
#include"graphics/swap_chain.hpp"
#include"graphics/pipeline.hpp"
#include "graphics/synchronisation/semaphore.hpp"
#include "graphics/command/command_pool.hpp"
#include "graphics/command/command_buffer.hpp"

namespace Graphics {
    class Renderer {
    public:
	Renderer(Swap_chain& _swap_chain, Pipeline& _pipeline);

	void draw_frame();

	Renderer(Renderer const&) = delete;
	Renderer& operator=(Renderer const&) = delete;

    private:
	Swap_chain* swap_chain;
	Pipeline* pipeline;

	Semaphore image_available_sem;
	Semaphore rendering_finished_sem;

	Command_pool graphics_command_pool;
	// One command buffer for each image in the swap chain
	std::vector<Command_buffer> graphics_command_buffers;


	void record_command_buffers();

    };
}

#endif
