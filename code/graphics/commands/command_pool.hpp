#pragma once

#include "utility/unique_handle.hpp"
#include "graphics/commands/command_buffer.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Command_pool final: public Utility::Unique_handle<VkCommandPool> {
    public:
	Command_pool(class Logical_device* _logical_device, std::uint32_t _queue_family_index);
	
	Command_buffer allocate_command_buffer();
    
    private:
	class Logical_device* logical_device;

    };
}
