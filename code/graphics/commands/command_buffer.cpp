#include "graphics/commands/command_buffer.hpp"
#include "graphics/commands/command_pool.hpp"
#include "graphics/devices/logical_device.hpp"
#include<array>

Graphics::Command_buffer::Command_buffer(VkCommandBuffer _command_buffer, class Command_pool* _command_pool, class Logical_device* _logical_device):
    Unique_handle(
	_command_buffer, 
	[_command_pool, _logical_device](Handle _command_buffer) { 
	    std::array<VkCommandBuffer> command_buffers = { _command_buffer };
	    vkFreeCommandBuffers(*_logical_device, _command_pool, command_buffers.size(), command_buffers.data());
	}
    }
{ }
