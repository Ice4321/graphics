#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

namespace Graphics {
    class Physical_device: public Utility::Unique_handle<VkPhysicalDevice> {
    public:
	static std::vector<Physical_device> enumerate_all();
	
	Physical_device(Handle _physical_device);

	VkPhysicalDeviceProperties const& get_properties() const noexcept;

	std::uint32_t get_queue_family_count() const noexcept;
	VkQueueFamilyProperties const& get_queue_family_properties(std::uint32_t _queue_index) const;
	
	bool get_queue_family_presentation_support(class Surface& _surface, std::uint32_t _queue_family_index);
	VkSurfaceCapabilitiesKHR get_surface_capabilities(class Surface& _surface);
	std::vector<VkSurfaceFormatKHR> get_surface_formats(class Surface& _surface);
	std::vector<VkPresentModeKHR> get_surface_presentation_modes(class Surface& _surface);

    private:
	VkPhysicalDeviceProperties properties;
	std::vector<VkQueueFamilyProperties> queue_family_properties;

    };
}
