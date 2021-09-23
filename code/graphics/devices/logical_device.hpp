#pragma once

#include<vulkan/vulkan.h>
#include"graphics/devices/physical_device.hpp"
#include"graphics/surface.hpp"
#include"utility/unique_handle.hpp"
#include"graphics/commands/queue.hpp"
#include<vector>

namespace Graphics {
    class Logical_device: public Utility::Unique_handle<VkDevice> {
    public:
	Logical_device(Physical_device& _physical_device, Surface& _surface);
	
	// TODO: Change this!! Must be by const ref, but then it can't be used to submit commands...
	Queue& get_graphics_queue() noexcept;
	Queue& get_presentation_queue() noexcept;

	void wait_idle();

	void TEST() {}

    private:
	// These might be the same queue
	Queue graphics_queue;
	Queue presentation_queue;
	

    };
}
