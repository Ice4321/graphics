#pragma once

#include "utility/unique_handle.hpp"
#include <cstdint>
#include <vulkan/vulkan.h>
#include <span>

namespace Graphics {
    class Queue: public Utility::Unique_handle<VkQueue> {
    public:
	static VkDeviceQueueCreateInfo make_creation_info(std::uint32_t _family_index, std::uint32_t _count);

	Queue() = default;
	Queue(class Logical_device& _logical_device, std::uint32_t _family_index, std::uint32_t _index);

	std::uint32_t get_family_index() const noexcept;
	std::uint32_t get_index() const noexcept;

	void submit(class Command_buffer& _command_buffer, class Semaphore& _wait_sem, class Semaphore& _signal_sem, VkPipelineStageFlags _wait_stages);
	void present(class Swap_chain& _swap_chain, std::uint32_t _swap_chain_image_index, class Semaphore& _wait_sem);

    private:
	// family_index corresponds to the index of an element of the pQueueFamilyProperties array that was returned by vkGetPhysicalDeviceQueueFamilyProperties
	std::uint32_t family_index;
	std::uint32_t index;

    };
}
