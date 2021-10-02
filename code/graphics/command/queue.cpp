#include "graphics/command/queue.hpp"
#include "preprocessor/macros.hpp"
#include "graphics/command/command_buffer.hpp"
#include "graphics/synchronisation/semaphore.hpp"
#include "graphics/utility/vulkan_assert.hpp"
#include "graphics/device/logical.hpp"
#include "graphics/wsi/swap_chain.hpp"

Graphics::Queue::Queue(Logical_device& _logical_device, std::uint32_t _family_index, std::uint32_t _index):
    family_index(_family_index), index(_index)
{ 
    Handle queue;
    vkGetDeviceQueue(_logical_device, family_index, index, &queue);
    Unique_handle::operator=({queue});
}

std::uint32_t Graphics::Queue::get_index() const noexcept {
    IF_DEBUG(ASSERT(has_value()));
    return index;
}

std::uint32_t Graphics::Queue::get_family_index() const noexcept {
    IF_DEBUG(ASSERT(has_value()));
    return family_index;
}

void Graphics::Queue::submit(Command_buffer& _command_buffer, Semaphore& _wait_sem, Semaphore& _signal_sem, VkPipelineStageFlags _wait_stages) {
    VkCommandBuffer command_buffers[] = { _command_buffer };
    VkPipelineStageFlags wait_stages[] = { _wait_stages };
    VkSemaphore wait_semaphores[] = { _wait_sem };
    VkSemaphore signal_semaphores[] = { _signal_sem };

    VkSubmitInfo submit_info[] = {{
	.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
	.pNext = nullptr,
	.waitSemaphoreCount = 1,
	.pWaitSemaphores = wait_semaphores,
	.pWaitDstStageMask = wait_stages,
	.commandBufferCount = 1,
	.pCommandBuffers = command_buffers,
	.signalSemaphoreCount = 1,
	.pSignalSemaphores = signal_semaphores
    }};

    VULKAN_ASSERT(vkQueueSubmit(*this, 1, submit_info, VK_NULL_HANDLE)); 
}

void Graphics::Queue::present(Swap_chain& _swap_chain, std::uint32_t _swap_chain_image_index, Semaphore& _wait_sem) {
    VkSwapchainKHR swap_chains[] = { _swap_chain };
    std::uint32_t image_indices[] = { _swap_chain_image_index };
    VkSemaphore wait_semaphores[] = { _wait_sem };

    VkPresentInfoKHR present_info{
	.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
	.pNext = nullptr,
	.waitSemaphoreCount = 1,
	.pWaitSemaphores = wait_semaphores,
	.swapchainCount = 1,
	.pSwapchains = swap_chains,
	.pImageIndices = image_indices,
	.pResults = nullptr
    };

    VULKAN_ASSERT(vkQueuePresentKHR(*this, &present_info)); 
}

VkDeviceQueueCreateInfo Graphics::Queue::make_creation_info(std::uint32_t _family_index, std::uint32_t _count) {
    // The lifetime of this array must continue after exiting this function, because it's referenced in VkDeviceQueueCreateInfo
    static std::vector<float> const priorities(64, 1.0f);

    return {
	.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.queueFamilyIndex = _family_index,
	.queueCount = _count,
	.pQueuePriorities = priorities.data()
    };

}
