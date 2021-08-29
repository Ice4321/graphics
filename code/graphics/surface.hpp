#ifndef INCLUDED_GRAPHICS_SURFACE_HPP
#define INCLUDED_GRAPHICS_SURFACE_HPP

#include<vulkan/vulkan.h>
#include"graphics/window.hpp"
#include"graphics/instance.hpp"

namespace Graphics {
    class Surface {
    public:
	// Vulkan instance must be within its lifetime during Surface's destruction
	Surface(Instance& _instance, Window& _window);

	~Surface();

	Surface(Surface const&) = delete;
	Surface& operator=(Surface const&) = delete;

    private:
	VkSurfaceKHR surface;
	Instance& instance;

    };
}

#endif
