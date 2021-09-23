#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Command_buffer: public Utility::Unique_handle<VkCommandBuffer> {
    public:
	// TODO: Can this be made accessible only to Command_pool::allocate_command_buffer()?
	Command_buffer(VkCommandBuffer _command_buffer, class Command_pool* _command_pool, class Logical_device* _logical_device);
    };
}
