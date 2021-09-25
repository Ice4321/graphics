#ifndef INCLUDED_GRAPHICS_SURFACE_HPP
#define INCLUDED_GRAPHICS_SURFACE_HPP

#include<vulkan/vulkan.h>
#include"graphics/wsi/window.hpp"
#include"graphics/instance/instance.hpp"
#include"graphics/devices/physical_device.hpp"

namespace Graphics {
    class Surface {
    public:
	// Instance must be within its lifetime during Surface's destruction
	// Instance's address must not change until Surface is destroyed (no copying or moving)
	Surface(Instance& _instance, Window& _window);

	operator VkSurfaceKHR& () noexcept;

	bool test_queue_family_presentation_support(Physical_device& _physical_device, std::uint32_t _queue_family_index);

	~Surface();

	Surface(Surface const&) = delete;
	Surface& operator=(Surface const&) = delete;

    private:
	VkSurfaceKHR surface;
	Instance* instance;

    };
}

#endif
