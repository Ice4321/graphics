#ifndef INCLUDED_GRAPHICS_SWAP_CHAIN_HPP
#define INCLUDED_GRAPHICS_SWAP_CHAIN_HPP

#include<vulkan/vulkan.h>
#include<vector>
#include"graphics/physical_device.hpp"
#include"graphics/logical_device.hpp"
#include"graphics/surface.hpp"
#include"graphics/window.hpp"


namespace Graphics {
    class Swap_chain {
    public:
	Swap_chain(Physical_device& _physical_device, Logical_device& _logical_device, Surface& _surface, Window& _window);

	operator VkSwapchainKHR& () noexcept;

	~Swap_chain();

	Swap_chain(Swap_chain const&) = delete;
	Swap_chain& operator=(Swap_chain const&) = delete;

    private:
	VkSwapchainKHR swap_chain;
	std::vector<VkImage> images;
	VkSurfaceFormatKHR image_format;
	VkExtent2D image_extent;
	Logical_device* logical_device;

    };
}

#endif
