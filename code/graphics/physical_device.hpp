#ifndef INCLUDED_GRAPHICS_PHYSICAL_DEVICE_HPP
#define INCLUDED_GRAPHICS_PHYSICAL_DEVICE_HPP

#include<vulkan/vulkan.h>
#include<vector>
#include"graphics/instance.hpp"

namespace Exceptions {
    namespace Graphics {
	namespace Physical_device {
	    struct Queue_family_not_found: public std::exception { };
	}
    }
}

namespace Graphics {
    class Physical_device {
    public:
	static std::vector<Physical_device> enumerate_all(Instance& _instance);
	
	Physical_device(VkPhysicalDevice _physical_device) noexcept;

	operator VkPhysicalDevice& () noexcept;

	VkPhysicalDeviceProperties const& get_properties() const noexcept;
	std::size_t get_queue_family_index(VkQueueFlagBits _capabilities) const;

	~Physical_device() = default;

	Physical_device(Physical_device const&) = delete;
	Physical_device& operator=(Physical_device const&) = delete;

    private:
	VkPhysicalDevice physical_device;
	VkPhysicalDeviceProperties properties;
	std::vector<VkQueueFamilyProperties> queue_family_properties;

    };
}

#endif
