#ifndef INCLUDED_GRAPHICS_PHYSICAL_DEVICE_HPP
#define INCLUDED_GRAPHICS_PHYSICAL_DEVICE_HPP

#include<vulkan/vulkan.h>
#include<vector>

namespace Graphics {
    class Physical_device {
    public:
	static std::vector<Physical_device> enumerate_all(VkInstance _instance);
	
	operator VkPhysicalDevice& () noexcept;

	Physical_device(Physical_device const&) = delete;
	Physical_device& operator=(Physical_device const&) = delete;

    private:
	VkPhysicalDevice physical_device;

	Physical_device(VkPhysicalDevice _physical_device) noexcept;

    };
}

#endif
