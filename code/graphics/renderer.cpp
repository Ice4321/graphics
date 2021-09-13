#include"graphics/renderer.hpp"

Graphics::Renderer::Renderer(Logical_device& _logical_device, Swap_chain& _swap_chain, Pipeline& _pipeline):
    logical_device(&_logical_device),
    swap_chain(&_swap_chain),
    pipeline(&_pipeline)
{
    VkCommandPoolCreateInfo graphics_command_pool_create_info{
	.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.queueFamilyIndex = logical_device->get_graphics_queue_index()
    };

    if(vkCreateCommandPool(*logical_device, &graphics_command_pool_create_info, nullptr, &graphics_command_pool) < 0) critical_error("vkCreateCommandPool()");

    VkCommandBufferAllocateInfo graphics_command_buffer_allocate_info{
	.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	.pNext = nullptr,
	.commandPool = graphics_command_pool,
	.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
	.commandBufferCount = (std::uint32_t)swap_chain->get_image_count()
    };
    
    graphics_command_buffers.resize(swap_chain->get_image_count());
    if(vkAllocateCommandBuffers(*logical_device, &graphics_command_buffer_allocate_info, graphics_command_buffers.data()) < 0) {
	critical_error("vkAllocateCommandBuffers()");
    }

    record_command_buffers();

    VkSemaphoreCreateInfo semaphore_create_info{
	.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
    };

    if(vkCreateSemaphore(*logical_device, &semaphore_create_info, nullptr, &image_available_sem) < 0) critical_error("vkCreateSemaphore()");
    if(vkCreateSemaphore(*logical_device, &semaphore_create_info, nullptr, &rendering_finished_sem) < 0) critical_error("vkCreateSemaphore()");


}

Graphics::Renderer::~Renderer() {
    vkDestroySemaphore(*logical_device, rendering_finished_sem, nullptr);
    vkDestroySemaphore(*logical_device, image_available_sem, nullptr);
    vkFreeCommandBuffers(*logical_device, graphics_command_pool, graphics_command_buffers.size(), graphics_command_buffers.data());
    vkDestroyCommandPool(*logical_device, graphics_command_pool, nullptr);
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

    if(vkQueueSubmit(logical_device->get_graphics_queue(), 1, queue_submit_info, VK_NULL_HANDLE) < 0) critical_error("vkQueueSubmit()");

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

    if(vkQueuePresentKHR(logical_device->get_presentation_queue(), &present_info) < 0) critical_error("vkQueuePresentKHR()");
}


void Graphics::Renderer::record_command_buffers() {
    for(std::size_t i = 0; i< graphics_command_buffers.size(); ++i) {
	VkCommandBufferBeginInfo begin_info{
	    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	    .pNext = nullptr,
	    .flags = 0,
	    .pInheritanceInfo = nullptr
	};

	if(vkBeginCommandBuffer(graphics_command_buffers[i], &begin_info) < 0) critical_error("vkBeginCommandBuffer()");
	
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

	if(vkEndCommandBuffer(graphics_command_buffers[i]) < 0) critical_error("vkEndCommandBuffer()");

    }
}