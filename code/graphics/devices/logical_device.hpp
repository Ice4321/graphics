#ifndef INCLUDED_GRAPHICS_LOGICAL_DEVICE_HPP
#define INCLUDED_GRAPHICS_LOGICAL_DEVICE_HPP

#include<vulkan/vulkan.h>
#include"graphics/devices/physical_device.hpp"
#include"graphics/surface.hpp"
#include"utility/unique_handle.hpp"
#include"graphics/devices/device_queue.hpp"
#include<vector>

namespace Graphics {
    class Logical_device: public Utility::Unique_handle<VkDevice> {
    public:
	Logical_device(Physical_device& _physical_device, Surface& _surface);
	
	// TODO: Change this!! Must be by const ref, but then it can't be used to submit commands...
	Device_queue& get_graphics_queue() noexcept;
	Device_queue& get_presentation_queue() noexcept;

	void wait_idle();

    private:
	// These might be the same queue
	Device_queue graphics_queue;
	Device_queue presentation_queue;
	

    };
}

#endif
