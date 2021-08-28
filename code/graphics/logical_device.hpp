#ifndef INCLUDED_GRAPHICS_LOGICAL_DEVICE_HPP
#define INCLUDED_GRAPHICS_LOGICAL_DEVICE_HPP

#include<vulkan/vulkan.h>
#include"graphics/physical_device.hpp"

namespace Graphics {
    class Logical_device {
    public:
	Logical_device(Physical_device& _physical_device);
	
	~Logical_device();

	Logical_device(Logical_device const&) = delete;
	Logical_device& operator=(Logical_device const&) = delete;

    private:
	VkDevice logical_device;

    };
}

#endif
