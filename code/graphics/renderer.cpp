#include"graphics/renderer.hpp"
#include"graphics/utility/vulkan_assert.hpp"

Graphics::Renderer::Renderer(Logical_device& _logical_device, Swap_chain& _swap_chain, Pipeline& _pipeline):
    logical_device(&_logical_device),
    swap_chain(&_swap_chain),
    pipeline(&_pipeline),
    image_available_sem(&_logical_device),
    rendering_finished_sem(&_logical_device),
    graphics_command_pool(&_logical_device, _logical_device.get_graphics_queue().get_family_index())
{
    graphics_command_buffers.reserve(swap_chain->get_image_count());
    for(std::size_t i = 0; i < swap_chain->get_image_count(); ++i) {
	graphics_command_buffers.emplace_back(graphics_command_pool.allocate_command_buffer());
    }

    record_command_buffers();

}

void Graphics::Renderer::draw_frame() {
    // TODO: get rid of arrays and code duplication
    
    std::uint32_t acquired_image_index = swap_chain->acquire_next_image(image_available_sem);

    VkCommandBuffer submit_command_buffers[] = { graphics_command_buffers[acquired_image_index] };
    VkSemaphore wait_semaphores[] = { image_available_sem };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSemaphore signal_semaphores[] = { rendering_finished_sem };

    VkSubmitInfo queue_submit_info[] = {{
	.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
	.pNext = nullptr,
	.waitSemaphoreCount = 1,
	.pWaitSemaphores = wait_semaphores,
	.pWaitDstStageMask = wait_stages,
	.commandBufferCount = 1,
	.pCommandBuffers = submit_command_buffers,
	.signalSemaphoreCount = 1,
	.pSignalSemaphores = signal_semaphores
    }};

    VULKAN_ASSERT(vkQueueSubmit(logical_device->get_graphics_queue(), 1, queue_submit_info, VK_NULL_HANDLE)); 

    VkSwapchainKHR swap_chains[] = { *swap_chain };
    std::uint32_t image_indices[] = { acquired_image_index };

    VkPresentInfoKHR present_info{
	.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
	.pNext = nullptr,
	.waitSemaphoreCount = 1,
	.pWaitSemaphores = signal_semaphores,
	.swapchainCount = 1,
	.pSwapchains = swap_chains,
	.pImageIndices = image_indices,
	.pResults = nullptr
    };

    VULKAN_ASSERT(vkQueuePresentKHR(logical_device->get_presentation_queue(), &present_info)); 
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
