#pragma once

#include "utility/unique_handle.hpp"
#include "graphics/command/command_buffer.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Command_pool: public Utility::Unique_handle<VkCommandPool> {
    public:
	Command_pool(std::uint32_t _queue_family_index);
	
	Command_buffer allocate_command_buffer();

    };
}
