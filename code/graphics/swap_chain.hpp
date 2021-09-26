#pragma once

#include "utility/unique_handle.hpp"
#include "graphics/image/image.hpp"
#include "graphics/image/view.hpp"
#include <vulkan/vulkan.h>


namespace Graphics { class Pipeline; }
#include<vulkan/vulkan.h>
#include<vector>
#include"graphics/wsi/surface.hpp"
#include"graphics/wsi/window.hpp"


namespace Graphics {
    class Swap_chain: public Utility::Unique_handle<VkSwapchainKHR> {
    public:
	Swap_chain(Surface& _surface, Window& _window);

	void create_framebuffers(Pipeline& _pipeline);
	
	std::size_t get_image_count() const noexcept;
	VkExtent2D get_image_extent() const noexcept;
	VkFormat get_image_format() const noexcept;
	std::vector<VkFramebuffer>& get_framebuffers() noexcept;
	
	// Returns the index of the acquired image, as obtained from vkGetSwapchainImagesKHR()
	std::uint32_t acquire_next_image(VkSemaphore _semaphore);
	~Swap_chain();

    private:
	VkFormat image_format;
	VkExtent2D image_extent;
	std::vector<Image> images;
	std::vector<Image_view> image_views;
	std::vector<VkFramebuffer> framebuffers; // TODO: remove this

    };
}

