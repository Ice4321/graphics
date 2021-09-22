#include "graphics/synchronisation/semaphore.hpp"
#include "graphics/devices/logical_device.hpp"
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Semaphore::Semaphore(class Logical_device* _logical_device) {
    VkSemaphoreCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
    };

    VkSemaphore semaphore;
    VULKAN_ASSERT(vkCreateSemaphore(*_logical_device, &create_info, nullptr, &semaphore));
    Unique_handle::operator=({semaphore, [_logical_device](Handle _semaphore) { vkDestroySemaphore(*_logical_device, _semaphore, nullptr); }});
}
