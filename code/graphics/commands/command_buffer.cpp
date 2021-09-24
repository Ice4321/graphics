#include "graphics/commands/command_buffer.hpp"
#include "graphics/commands/command_pool.hpp"
#include "graphics/devices/logical_device.hpp"

Graphics::Command_buffer::Command_buffer(VkCommandBuffer _command_buffer, Command_pool* _command_pool, Logical_device* _logical_device):
    Unique_handle(
	_command_buffer, 
	[_command_pool, _logical_device](Handle _command_buffer) { 
	    VkCommandBuffer command_buffers[] = { _command_buffer };
	    vkFreeCommandBuffers(*_logical_device, *_command_pool, 1, command_buffers);
	}
    )
{ }
