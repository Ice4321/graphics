#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Command_buffer: public Utility::Unique_handle<VkCommandBuffer> {
    public:
	// Command_buffer must not outlive the VkCommandPool object of _command_pool
	Command_buffer(Handle _command_buffer, class Command_pool& _command_pool);

	void begin_recording();
	void end_recording();
	
    };
}
