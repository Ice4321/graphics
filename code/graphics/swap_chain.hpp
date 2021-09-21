#ifndef INCLUDED_GRAPHICS_SWAP_CHAIN_HPP
#define INCLUDED_GRAPHICS_SWAP_CHAIN_HPP

namespace Graphics { class Pipeline; }
#include<vulkan/vulkan.h>
#include<vector>
#include"graphics/devices/physical_device.hpp"
#include"graphics/devices/logical_device.hpp"
#include"graphics/surface.hpp"
#include"graphics/window.hpp"


namespace Graphics {
    class Swap_chain {
    public:
	Swap_chain(Physical_device& _physical_device, Logical_device& _logical_device, Surface& _surface, Window& _window);

	void create_framebuffers(Pipeline& _pipeline);

	operator VkSwapchainKHR& () noexcept;
	
	std::size_t get_image_count() const noexcept;
	VkExtent2D const& get_image_extent() const noexcept;
	VkFormat const& get_image_format() const noexcept;
	std::vector<VkFramebuffer>& get_framebuffers() noexcept;
	
	// Returns the index of the acquired image, as obtained from vkGetSwapchainImagesKHR()
	std::uint32_t acquire_next_image(VkSemaphore _semaphore);

	~Swap_chain();

	Swap_chain(Swap_chain const&) = delete;
	Swap_chain& operator=(Swap_chain const&) = delete;

    private:
	VkSwapchainKHR swap_chain;
	std::vector<VkImage> images;
	std::vector<VkImageView> image_views;
	std::vector<VkFramebuffer> framebuffers;
	VkFormat image_format;
	VkExtent2D image_extent;
	Logical_device* logical_device;

    };
}

#endif
