#include"graphics/swap_chain.hpp"
#include"graphics/pipeline.hpp"
#include"graphics/utility/vulkan_assert.hpp"
#include<algorithm>
#include<limits>


Graphics::Swap_chain::Swap_chain(Logical_device* _logical_device, Physical_device& _physical_device, Surface& _surface, Window& _window):
    logical_device(_logical_device)
{
    VkSurfaceCapabilitiesKHR surface_capabilities = _physical_device.get_surface_capabilities(_surface);
    std::vector<VkSurfaceFormatKHR> surface_formats = _physical_device.get_surface_formats(_surface);


    std::uint32_t surface_presentation_mode_count;
    VULKAN_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &surface_presentation_mode_count, nullptr));

    std::vector<VkPresentModeKHR> surface_presentation_modes(surface_presentation_mode_count);
    VULKAN_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &surface_presentation_mode_count, surface_presentation_modes.data()));
    
    VkSurfaceFormatKHR chosen_format;
    VkPresentModeKHR chosen_presentation_mode;

    {
	bool found = false;
	for(auto const& format : surface_formats) {
	    if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
		chosen_format = format;
		found = true;
		break;
	    }
	}
	ASSERT(found);
    }

    {
	bool found = false;
	for(auto const& mode : surface_presentation_modes) {
	    if(mode == VK_PRESENT_MODE_MAILBOX_KHR) {
		chosen_presentation_mode = mode;
		found = true;
		break;
	    }
	}
	ASSERT(found);
    }
    
    VkExtent2D chosen_extent;
    if(surface_capabilities.currentExtent.width == 0xFFFFFFFF && surface_capabilities.currentExtent.height == 0xFFFFFFFF) {
	// The surface size will be determined by the extent of a swapchain targeting the surface
	chosen_extent.width = std::clamp<std::uint32_t>(_window.get_framebuffer_width_px(), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
	chosen_extent.height = std::clamp<std::uint32_t>(_window.get_framebuffer_height_px(), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);
    } else {
	// surface_capabilities.currentExtent is the current width and height of the surface
	chosen_extent = surface_capabilities.currentExtent;
    }

    std::uint32_t min_image_count = surface_capabilities.minImageCount + (
	surface_capabilities.maxImageCount == 0 || surface_capabilities.maxImageCount > surface_capabilities.minImageCount ? 1 : 0
    );

    std::vector<std::uint32_t> queue_families;
    queue_families.emplace_back(logical_device->get_graphics_queue().get_family_index());
    if(logical_device->get_graphics_queue().get_family_index() != logical_device->get_presentation_queue().get_family_index()) {
	queue_families.emplace_back(logical_device->get_presentation_queue().get_family_index());
    }

    VkSwapchainCreateInfoKHR create_info{
	.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
	.pNext = nullptr,
	.flags = 0,
	.surface = _surface,
	.minImageCount = min_image_count,
	.imageFormat = chosen_format.format,
	.imageColorSpace = chosen_format.colorSpace,
	.imageExtent = chosen_extent,
	.imageArrayLayers = 1,
	.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	.imageSharingMode = queue_families.size() > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
	.queueFamilyIndexCount = (std::uint32_t)queue_families.size(), // Ignored if sharing mode is exclusive
	.pQueueFamilyIndices = queue_families.data(), // Ignored if sharing mode is exclusive
	.preTransform = surface_capabilities.currentTransform,
	.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	.presentMode = chosen_presentation_mode,
	.clipped = VK_TRUE,
	.oldSwapchain = VK_NULL_HANDLE
    };
    
    VkSwapchainKHR swap_chain;
    VULKAN_ASSERT(vkCreateSwapchainKHR(*logical_device, &create_info, nullptr, &swap_chain)); 
    Unique_handle::operator=({swap_chain, [_logical_device](Handle _swap_chain) { vkDestroySwapchainKHR(*_logical_device, _swap_chain, nullptr); }});
    
    image_format = chosen_format.format;
    image_extent = chosen_extent;

    // Application must not destroy these images
    std::uint32_t actual_image_count;
    VULKAN_ASSERT(vkGetSwapchainImagesKHR(*logical_device, swap_chain, &actual_image_count, nullptr)); 
    images.resize(actual_image_count);
    VULKAN_ASSERT(vkGetSwapchainImagesKHR(*logical_device, swap_chain, &actual_image_count, images.data())); 
    
    image_views.resize(images.size());

    for(std::size_t i = 0; i < image_views.size(); ++i) {
	VkImageViewCreateInfo view_create_info{
	    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
	    .pNext = nullptr,
	    .flags = 0,
	    .image = images[i],
	    .viewType = VK_IMAGE_VIEW_TYPE_2D,
	    .format = image_format,
	    .components = {
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY
	    },
	    .subresourceRange = {
		.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.baseMipLevel = 0,
		.levelCount = 1,
		.baseArrayLayer = 0,
		.layerCount = 1
	    }
	};

	VULKAN_ASSERT(vkCreateImageView(*logical_device, &view_create_info, nullptr, &image_views[i])); 
    }
}

Graphics::Swap_chain::~Swap_chain() {
    for(auto& framebuffer : framebuffers) vkDestroyFramebuffer(*logical_device, framebuffer, nullptr);
    for(auto& image_view : image_views) vkDestroyImageView(*logical_device, image_view, nullptr);
    //vkDestroySwapchainKHR(*logical_device, swap_chain, nullptr);
}

void Graphics::Swap_chain::create_framebuffers(Pipeline& _pipeline) {
    framebuffers.resize(images.size());

    for(std::size_t i = 0; i < image_views.size(); ++i) {
	VkImageView attachments[] = {
	    image_views[i]
	};

	VkFramebufferCreateInfo framebuffer_create_info{
	    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
	    .pNext = nullptr,
	    .flags = 0,
	    .renderPass = _pipeline.get_render_pass(),
	    .attachmentCount = 1,
	    .pAttachments = attachments,
	    .width = image_extent.width,
	    .height = image_extent.height,
	    .layers = 1
	};

	VULKAN_ASSERT(vkCreateFramebuffer(*logical_device, &framebuffer_create_info, nullptr, &framebuffers[i])); 
    }
}

std::size_t Graphics::Swap_chain::get_image_count() const noexcept {
    return images.size();
}

VkExtent2D const& Graphics::Swap_chain::get_image_extent() const noexcept {
    return image_extent;
}


VkFormat const& Graphics::Swap_chain::get_image_format() const noexcept {
    return image_format;
}

std::vector<VkFramebuffer>& Graphics::Swap_chain::get_framebuffers() noexcept {
    return framebuffers;
}

std::uint32_t Graphics::Swap_chain::acquire_next_image(VkSemaphore _semaphore) {
    std::uint32_t index;
    VULKAN_ASSERT(vkAcquireNextImageKHR(*logical_device, swap_chain, std::numeric_limits<std::uint64_t>::max(), _semaphore, VK_NULL_HANDLE, &index));

    return index;
}



