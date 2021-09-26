#include "graphics/synchronisation/semaphore.hpp"
#include "graphics/utility/vulkan_assert.hpp"
#include "graphics/state/globals.hpp"

Graphics::Semaphore::Semaphore() {
    VkSemaphoreCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
    };

    Handle semaphore;
    VULKAN_ASSERT(vkCreateSemaphore(*logical_device, &create_info, nullptr, &semaphore));
    Unique_handle::operator=({semaphore, [](Handle _semaphore) { vkDestroySemaphore(*logical_device, _semaphore, nullptr); }});
}
