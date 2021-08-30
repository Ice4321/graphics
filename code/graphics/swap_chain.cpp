#include"graphics/swap_chain.hpp"
#include<algorithm>


Graphics::Swap_chain::Swap_chain(Physical_device& _physical_device, Logical_device& _logical_device, Surface& _surface, Window& _window):
    logical_device(&_logical_device)
{
    VkSurfaceCapabilitiesKHR surface_capabilities;
    if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physical_device, _surface, &surface_capabilities) < 0) {
	critical_error("vkGetPhysicalDeviceSurfaceCapabilitiesKHR()");
    }

    std::uint32_t surface_format_count;
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(_physical_device, _surface, &surface_format_count, nullptr) < 0) {
	critical_error("vkGetPhysicalDeviceSurfaceFormatsKHR()");
    }
    std::vector<VkSurfaceFormatKHR> surface_formats(surface_format_count);
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(_physical_device, _surface, &surface_format_count, surface_formats.data()) < 0) {
	critical_error("vkGetPhysicalDeviceSurfaceFormatsKHR()");
    }


    std::uint32_t surface_presentation_mode_count;
    if(vkGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &surface_presentation_mode_count, nullptr) < 0) {
	critical_error("vkGetPhysicalDeviceSurfacePresentModesKHR()");
    }
    std::vector<VkPresentModeKHR> surface_presentation_modes(surface_presentation_mode_count);
    if(vkGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &surface_presentation_mode_count, surface_presentation_modes.data()) < 0) {
	critical_error("vkGetPhysicalDeviceSurfacePresentModesKHR()");
    }
    
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
	if(!found) critical_error("Surface format not available");
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
	if(!found) critical_error("Surface presentation mode not available");
    }
    
    VkExtent2D chosen_extent;
    if(surface_capabilities.currentExtent.width == 0xFFFFFFFF && surface_capabilities.currentExtent.height == 0xFFFFFFFF) {
	// The surface size will be determined by the extent of a swapchain targeting the surface
	chosen_extent.width = std::clamp<std::uint32_t>(_window.get_width_px(), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
	chosen_extent.height = std::clamp<std::uint32_t>(_window.get_height_px(), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);
    } else {
	// surface_capabilities.currentExtent is the current width and height of the surface
	chosen_extent = surface_capabilities.currentExtent;
    }

    std::uint32_t image_count = surface_capabilities.minImageCount + (
	surface_capabilities.maxImageCount == 0 || surface_capabilities.maxImageCount > surface_capabilities.minImageCount ? 1 : 0
    );

    // TODO: VK_SHARING_MODE_EXCLUSIVE requires ownership transfer if graphics and presentation queues are not the same queue
    
    // queueFamilyIndexCount and pQueueFamilyIndices are zero if imageSharingMode is VK_SHARING_MODE_EXCLUSIVE
    VkSwapchainCreateInfoKHR create_info{
	.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
	.pNext = nullptr,
	.flags = 0,
	.surface = _surface,
	.minImageCount = image_count,
	.imageFormat = chosen_format.format,
	.imageColorSpace = chosen_format.colorSpace,
	.imageExtent = chosen_extent,
	.imageArrayLayers = 1,
	.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
	.queueFamilyIndexCount = 0,
	.pQueueFamilyIndices = nullptr,
	.preTransform = surface_capabilities.currentTransform,
	.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	.presentMode = chosen_presentation_mode,
	.clipped = VK_TRUE,
	.oldSwapchain = VK_NULL_HANDLE
    };

    if(vkCreateSwapchainKHR(*logical_device, &create_info, nullptr, &swap_chain) < 0) critical_error("vkCreateSwapchainKHR()");

}

Graphics::Swap_chain::operator VkSwapchainKHR& () noexcept {
    return swap_chain;
}

Graphics::Swap_chain::~Swap_chain() {
    vkDestroySwapchainKHR(*logical_device, swap_chain, nullptr);
}
