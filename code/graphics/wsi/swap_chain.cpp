#include "graphics/wsi/swap_chain.hpp"
#include "graphics/wsi/window.hpp"
#include "graphics/wsi/surface.hpp"
#include "graphics/device/physical.hpp"
#include "graphics/device/logical.hpp"
#include "graphics/utility/vulkan_assert.hpp"
#include "graphics/synchronisation/semaphore.hpp"
#include <algorithm>
#include <limits>


Graphics::Swap_chain::Swap_chain(Logical_device& _logical_device, Surface& _surface, Window& _window):
    logical_device(&_logical_device)
{
    VkSurfaceCapabilitiesKHR surface_capabilities = _logical_device.get_physical_device().get_surface_capabilities(_surface);
    std::vector<VkSurfaceFormatKHR> surface_formats = _logical_device.get_physical_device().get_surface_formats(_surface);
    std::vector<VkPresentModeKHR> surface_presentation_modes = _logical_device.get_physical_device().get_surface_presentation_modes(_surface);

    VkSurfaceFormatKHR chosen_format = {.format = VK_FORMAT_B8G8R8A8_SRGB, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    VkPresentModeKHR chosen_presentation_mode = VK_PRESENT_MODE_MAILBOX_KHR;
    VkExtent2D chosen_extent;
    
    ASSERT(std::ranges::find_if(surface_formats, [&](VkSurfaceFormatKHR const& _format) { 
	return _format.format == chosen_format.format && _format.colorSpace == chosen_format.colorSpace;
    }) != std::ranges::end(surface_formats));

    ASSERT(std::ranges::find(surface_presentation_modes, chosen_presentation_mode) != std::ranges::end(surface_presentation_modes));
    
    if(surface_capabilities.currentExtent.width == 0xFFFFFFFF && surface_capabilities.currentExtent.height == 0xFFFFFFFF) {
	// The surface size will be determined by the extent of a swapchain targetting the surface
	chosen_extent = {
	    .width = std::clamp<std::uint32_t>(_window.get_framebuffer_width_px(), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width),
	    .height = std::clamp<std::uint32_t>(_window.get_framebuffer_height_px(), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height)
	};
    } else {
	// surface_capabilities.currentExtent is the current width and height of the surface
	chosen_extent = surface_capabilities.currentExtent;
    }

    std::uint32_t min_image_count = surface_capabilities.minImageCount + (
	(surface_capabilities.maxImageCount == 0 || surface_capabilities.maxImageCount > surface_capabilities.minImageCount) ? 1 : 0
    );
    
    // Queue families that will share access to the swap chain
    std::vector<std::uint32_t> queue_families;
    queue_families.emplace_back(_logical_device.get_graphics_queue().get_family_index());
    if(_logical_device.get_graphics_queue().get_family_index() != _logical_device.get_presentation_queue().get_family_index()) {
	queue_families.emplace_back(_logical_device.get_presentation_queue().get_family_index());
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
    
    Handle swap_chain;
    VULKAN_ASSERT(vkCreateSwapchainKHR(_logical_device, &create_info, nullptr, &swap_chain)); 
    Unique_handle::operator=({swap_chain, [&_logical_device](Handle _swap_chain) { vkDestroySwapchainKHR(_logical_device, _swap_chain, nullptr); }});
    
    // These must be set before Image::get_swap_chain_images() is called
    image_format = chosen_format.format;
    image_extent = chosen_extent;

    images = Image::get_swap_chain_images(*logical_device, *this);
    
    image_views.reserve(images.size());
    for(auto& image : images) image_views.emplace_back(*logical_device, image);
}

std::size_t Graphics::Swap_chain::get_image_count() const noexcept {
    return images.size();
}

VkExtent2D Graphics::Swap_chain::get_image_extent() const noexcept {
    return image_extent;
}

VkFormat Graphics::Swap_chain::get_image_format() const noexcept {
    return image_format;
}

Graphics::Image_view& Graphics::Swap_chain::get_image_view(std::size_t _index) noexcept {
    return image_views[_index];
}

std::uint32_t Graphics::Swap_chain::acquire_next_image(Semaphore& _semaphore) {
    std::uint32_t index;
    VULKAN_ASSERT(vkAcquireNextImageKHR(*logical_device, *this, std::numeric_limits<std::uint64_t>::max(), _semaphore, VK_NULL_HANDLE, &index));
    return index;
}



