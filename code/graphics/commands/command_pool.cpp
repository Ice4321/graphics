#include "graphics/commands/command_pool.hpp"
#include "graphics/devices/logical_device.hpp"
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Command_pool::Command_pool(Logical_device* _logical_device, std::uint32_t _queue_family_index) {
    VkCommandPoolCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.queueFamilyIndex = _queue_family_index
    };

    VkCommandPool command_pool;
    VULKAN_ASSERT(vkCreateCommandPool(*_logical_device, &create_info, nullptr, &command_pool)); 
    Unique_handle::operator=({command_pool, [_logical_device](Handle _command_pool) { vkDestroyCommandPool(*_logical_device, _command_pool, nullptr); }});
}
