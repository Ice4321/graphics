#ifndef INCLUDED_GRAPHICS_PHYSICAL_DEVICE_HPP
#define INCLUDED_GRAPHICS_PHYSICAL_DEVICE_HPP

#include<vulkan/vulkan.h>
#include<vector>
#include<cstdint>
#include"graphics/instance.hpp"
#include"utility/unique_handle.hpp"

namespace Graphics {
    class Physical_device: public Utility::Unique_handle<VkPhysicalDevice> {
    public:
	static std::vector<Physical_device> enumerate_all(Instance& _instance);
	
	// TODO: Make this private
	Physical_device(VkPhysicalDevice _physical_device);

	VkPhysicalDeviceProperties const& get_properties() const noexcept;
	std::uint32_t get_queue_family_count() const noexcept;
	VkQueueFamilyProperties const& get_queue_family_properties(std::uint32_t _queue_index) const;

    private:
	VkPhysicalDeviceProperties properties;
	std::vector<VkQueueFamilyProperties> queue_family_properties;

    };
}

#endif
