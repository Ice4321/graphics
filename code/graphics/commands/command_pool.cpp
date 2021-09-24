#include "graphics/commands/command_pool.hpp"
#include "graphics/utility/vulkan_assert.hpp"
#include "graphics/devices/logical_device.hpp"

Graphics::Command_pool::Command_pool(Logical_device* _logical_device, std::uint32_t _queue_family_index):
    logical_device(_logical_device)
{
    VkCommandPoolCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.queueFamilyIndex = _queue_family_index
    };

    VkCommandPool command_pool;
    VULKAN_ASSERT(vkCreateCommandPool(*logical_device, &create_info, nullptr, &command_pool)); 
    Unique_handle::operator=({ command_pool, [_logical_device](Handle _command_pool) { vkDestroyCommandPool(*_logical_device, _command_pool, nullptr); }
    });
}

Graphics::Command_buffer Graphics::Command_pool::allocate_command_buffer() {
    VkCommandBufferAllocateInfo allocate_info{
	.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	.pNext = nullptr,
	.commandPool = *this,
	.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
	.commandBufferCount = 1
    };
    
    VkCommandBuffer command_buffers[1];
    // If the allocation of any of these command buffers fails, the implementation must free all successfully allocated command buffer objects
    VULKAN_ASSERT(vkAllocateCommandBuffers(*logical_device, &allocate_info, command_buffers));
    
    return { command_buffers[0], this, logical_device };
}
