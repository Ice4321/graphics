#ifndef INCLUDED_GRAPHICS_RENDERER_HPP
#define INCLUDED_GRAPHICS_RENDERER_HPP

#include<vulkan/vulkan.h>
#include"graphics/devices/logical_device.hpp"
#include"graphics/swap_chain.hpp"
#include"graphics/pipeline.hpp"
#include "graphics/synchronisation/semaphore.hpp"

namespace Graphics {
    class Renderer {
    public:
	Renderer(Logical_device& _logical_device, Swap_chain& _swap_chain, Pipeline& _pipeline);

	void draw_frame();

	~Renderer();

	Renderer(Renderer const&) = delete;
	Renderer& operator=(Renderer const&) = delete;

    private:
	Logical_device* logical_device;
	Swap_chain* swap_chain;
	Pipeline* pipeline;
	VkCommandPool graphics_command_pool;
	// One command buffer for each image in the swap chain
	std::vector<VkCommandBuffer> graphics_command_buffers;

	Semaphore image_available_sem;
	Semaphore rendering_finished_sem;

	void record_command_buffers();

    };
}

#endif
