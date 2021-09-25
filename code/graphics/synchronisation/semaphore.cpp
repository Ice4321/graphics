#include "graphics/synchronisation/semaphore.hpp"
#include "graphics/utility/vulkan_assert.hpp"
#include "graphics/devices/logical_device.hpp"

Graphics::Semaphore::Semaphore(Logical_device* _logical_device):
    logical_device(_logical_device) 
{
    VkSemaphoreCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
    };

    Handle semaphore;
    VULKAN_ASSERT(vkCreateSemaphore(*logical_device, &create_info, nullptr, &semaphore));
    Unique_handle::operator=({semaphore, [_logical_device](Handle _semaphore) { vkDestroySemaphore(*_logical_device, _semaphore, nullptr); }});
}
