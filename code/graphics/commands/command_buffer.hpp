#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Command_buffer: public Utility::Unique_handle<VkCommandBuffer> {
    public:
	Command_buffer(Handle _command_buffer, class Command_pool* _command_pool, class Logical_device* _logical_device);

	void begin_recording();
	void end_recording();

    private:
	class Command_pool* command_pool;
	class Logical_device* logical_device;
	
    };
}
