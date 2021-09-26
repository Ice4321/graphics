#include "graphics/command/command_buffer.hpp"
#include "graphics/command/command_pool.hpp"
#include "graphics/state/globals.hpp"
#include "graphics/utility/vulkan_assert.hpp"
#include <array>

Graphics::Command_buffer::Command_buffer(Handle _command_buffer, Command_pool& _command_pool) { 
    Unique_handle::operator=({_command_buffer, 
	[command_pool = _command_pool.get_handle()](Handle _command_buffer) { 
	    vkFreeCommandBuffers(*logical_device, command_pool, 1, std::array{_command_buffer}.data());
	}
    });
}


void Graphics::Command_buffer::begin_recording() {
    VkCommandBufferBeginInfo begin_info{
	.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	.pNext = nullptr,
	.flags = 0,
	.pInheritanceInfo = nullptr
    };

    VULKAN_ASSERT(vkBeginCommandBuffer(*this, &begin_info)); 
}

void Graphics::Command_buffer::end_recording() {
    VULKAN_ASSERT(vkEndCommandBuffer(*this)); 
}

