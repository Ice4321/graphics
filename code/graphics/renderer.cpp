#include"graphics/renderer.hpp"
#include"graphics/utility/vulkan_assert.hpp"
#include"graphics/device/logical.hpp"

Graphics::Renderer::Renderer(Logical_device& _logical_device, Swap_chain& _swap_chain, Pipeline& _pipeline):
    logical_device(&_logical_device),
    swap_chain(&_swap_chain),
    pipeline(&_pipeline),
    image_available_sem(_logical_device),
    rendering_finished_sem(_logical_device),
    graphics_command_pool(*logical_device, logical_device->get_graphics_queue().get_family_index())
{
    graphics_command_buffers.reserve(swap_chain->get_image_count());
    for(std::size_t i = 0; i < swap_chain->get_image_count(); ++i) {
	graphics_command_buffers.emplace_back(graphics_command_pool.allocate_command_buffer());
    }

    record_command_buffers();

}

void Graphics::Renderer::draw_frame() {
    std::uint32_t acquired_image_index = swap_chain->acquire_next_image(image_available_sem);
    logical_device->submit_drawing_commands(graphics_command_buffers[acquired_image_index], image_available_sem, rendering_finished_sem);
    logical_device->present(*swap_chain, acquired_image_index, rendering_finished_sem);
}


void Graphics::Renderer::record_command_buffers() {
    for(std::size_t i = 0; i< graphics_command_buffers.size(); ++i) {
	graphics_command_buffers[i].begin_recording();
	
	// See: VK_ATTACHMENT_LOAD_OP_CLEAR
	VkClearValue clear_values[] = {
	    { .color = { .float32 = { 0.0f, 0.0f, 0.0f, 1.0f } } }
	};

	VkRenderPassBeginInfo render_pass_begin_info{
	    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
	    .pNext = nullptr,
	    .renderPass = pipeline->get_render_pass(),
	    .framebuffer = swap_chain->get_framebuffers()[i],
	    .renderArea = {
		.offset = { .x = 0, .y = 0 },
		.extent = swap_chain->get_image_extent()
	    },
	    .clearValueCount = 1,
	    .pClearValues = clear_values
	};

	vkCmdBeginRenderPass(graphics_command_buffers[i], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(graphics_command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);
	vkCmdDraw(graphics_command_buffers[i], 3, 1, 0, 0);

	vkCmdEndRenderPass(graphics_command_buffers[i]);

	graphics_command_buffers[i].end_recording();

    }
}
