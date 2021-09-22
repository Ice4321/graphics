#pragma once

#include "utility/unique_handle.hpp"
#include <cstdint>
#include <vulkan/vulkan.h>

namespace Graphics {
    class Queue: public Utility::Unique_handle<VkQueue> {
    public:
	static VkDeviceQueueCreateInfo make_creation_info(std::uint32_t _family_index, std::uint32_t _count);

	Queue() = default;
	Queue(class Logical_device* _logical_device, std::uint32_t _family_index, std::uint32_t _index);

	std::uint32_t get_family_index() const noexcept;
	std::uint32_t get_index() const noexcept;

    private:
	// family_index corresponds to the index of an element of the pQueueFamilyProperties array that was returned by vkGetPhysicalDeviceQueueFamilyProperties
	std::uint32_t family_index;
	std::uint32_t index;

    };
}
