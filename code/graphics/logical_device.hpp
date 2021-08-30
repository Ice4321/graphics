#ifndef INCLUDED_GRAPHICS_LOGICAL_DEVICE_HPP
#define INCLUDED_GRAPHICS_LOGICAL_DEVICE_HPP

#include<vulkan/vulkan.h>
#include"graphics/physical_device.hpp"
#include"graphics/surface.hpp"
#include<vector>

namespace Graphics {
    class Logical_device {
    public:
	Logical_device(Physical_device& _physical_device, Surface& _surface);
	
	operator VkDevice& () noexcept;

	VkQueue get_graphics_queue() noexcept;
	VkQueue get_presentation_queue() noexcept;
	
	~Logical_device();

	Logical_device(Logical_device const&) = delete;
	Logical_device& operator=(Logical_device const&) = delete;

    private:
	VkDevice logical_device;

	// These might be the same queue
	VkQueue graphics_queue_handle;
	VkQueue presentation_queue_handle;
	
	// TODO: Change this to std::uint32_t, remove all casts, and use bool flags in create_queue_creation_info()
	int graphics_queue_family_index;
	int presentation_queue_family_index;

	// This function also sets graphics_queue_family_index and presentation_queue_family_index
	std::vector<VkDeviceQueueCreateInfo> create_queue_creation_info(Physical_device& _physical_device, Surface& _surface);
	

    };
}

#endif
