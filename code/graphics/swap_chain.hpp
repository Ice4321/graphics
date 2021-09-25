#pragma once

#include "utility/unique_handle.hpp"
#include "graphics/image/image.hpp"
#include "graphics/image/view.hpp"
#include <vulkan/vulkan.h>


namespace Graphics { class Pipeline; }
#include<vulkan/vulkan.h>
#include<vector>
#include"graphics/devices/physical_device.hpp"
#include"graphics/devices/logical_device.hpp"
#include"graphics/wsi/surface.hpp"
#include"graphics/wsi/window.hpp"


namespace Graphics {
    class Swap_chain: public Utility::Unique_handle<VkSwapchainKHR> {
    public:
	Swap_chain(class Logical_device* _logical_device, Physical_device& _physical_device, Surface& _surface, Window& _window);

	void create_framebuffers(Pipeline& _pipeline);
	
	std::size_t get_image_count() const noexcept;
	VkExtent2D const& get_image_extent() const noexcept;
	VkFormat const& get_image_format() const noexcept;
	std::vector<VkFramebuffer>& get_framebuffers() noexcept;
	
	// Returns the index of the acquired image, as obtained from vkGetSwapchainImagesKHR()
	std::uint32_t acquire_next_image(VkSemaphore _semaphore);
	~Swap_chain();

    private:
	class Logical_device* logical_device;

	VkFormat image_format;
	VkExtent2D image_extent;

	std::vector<Image> images;
	std::vector<Image_view> image_views;
	std::vector<VkFramebuffer> framebuffers;

    };
}

