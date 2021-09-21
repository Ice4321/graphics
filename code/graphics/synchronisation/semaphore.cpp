#include "graphics/synchronisation/semaphore.hpp"
#include "graphics/devices/logical_device.hpp"
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Semaphore::Semaphore(class Logical_device* _logical_device) {
    VkSemaphore semaphore;
    VkSemaphoreCreateInfo semaphore_create_info{
	.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
    };
    VULKAN_ASSERT(vkCreateSemaphore(*_logical_device, &semaphore_create_info, nullptr, &semaphore));
    Unique_handle::operator=({semaphore, [_logical_device](VkSemaphore _semaphore) { vkDestroySemaphore(*_logical_device, _semaphore, nullptr); }});
}
