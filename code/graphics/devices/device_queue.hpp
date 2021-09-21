#ifndef INCLUDED_GRAPHICS_DEVICES_DEVICE_QUEUE_HPP
#define INCLUDED_GRAPHICS_DEVICES_DEVICE_QUEUE_HPP

#include"utility/unique_handle.hpp"
#include<cstdint>
#include<vulkan/vulkan.h>

namespace Graphics {
    class Device_queue: public Utility::Unique_handle<VkQueue> {
    public:
	static VkDeviceQueueCreateInfo make_creation_info(std::uint32_t _family_index, std::uint32_t _count);

	Device_queue() = default;

	Device_queue(class Logical_device& _logical_device, std::uint32_t _family_index, std::uint32_t _index);

	std::uint32_t get_family_index() const noexcept;
	std::uint32_t get_index() const noexcept;

    private:
	// family_index corresponds to the index of an element of the pQueueFamilyProperties array that was returned by vkGetPhysicalDeviceQueueFamilyProperties
	std::uint32_t family_index;
	std::uint32_t index;

    };
}

#endif
