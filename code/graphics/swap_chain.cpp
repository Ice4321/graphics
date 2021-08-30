#include"graphics/swap_chain.hpp"
#include<algorithm>


Graphics::Swap_chain::Swap_chain(Physical_device& _physical_device, Surface& _surface, Window& _window) {
    VkSurfaceCapabilitiesKHR surface_capabilities;
    if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physical_device, _surface, &surface_capabilities) < 0) {
	critical_error("vkGetPhysicalDeviceSurfaceCapabilitiesKHR()");
    }

    uint32_t surface_format_count;
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(_physical_device, _surface, &surface_format_count, nullptr) < 0) {
	critical_error("vkGetPhysicalDeviceSurfaceFormatsKHR()");
    }
    std::vector<VkSurfaceFormatKHR> surface_formats(surface_format_count);
    if(vkGetPhysicalDeviceSurfaceFormatsKHR(_physical_device, _surface, &surface_format_count, surface_formats.data()) < 0) {
	critical_error("vkGetPhysicalDeviceSurfaceFormatsKHR()");
    }


    uint32_t surface_presentation_mode_count;
    if(vkGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &surface_presentation_mode_count, nullptr) < 0) {
	critical_error("vkGetPhysicalDeviceSurfacePresentModesKHR()");
    }
    std::vector<VkPresentModeKHR> surface_presentation_modes(surface_presentation_mode_count);
    if(vkGetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &surface_presentation_mode_count, surface_presentation_modes.data()) < 0) {
	critical_error("vkGetPhysicalDeviceSurfacePresentModesKHR()");
    }
    
    VkSurfaceFormatKHR chosen_surface_format;
    VkPresentModeKHR chosen_surface_presentation_mode;

    {
	bool found = false;
	for(auto const& format : surface_formats) {
	    if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
		chosen_surface_format = format;
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
		chosen_surface_presentation_mode = mode;
		found = true;
		break;
	    }
	}
	if(!found) critical_error("Surface presentation mode not available");
    }
    
    // surface_capabilities.currentExtent is the current width and height of the surface
    VkExtent2D chosen_extent;
    if(surface_capabilities.currentExtent.width == 0xFFFFFFFF && surface_capabilities.currentExtent.height == 0xFFFFFFFF) {
	// The surface size will be determined by the extent of a swapchain targeting the surface
	chosen_extent.width = std::clamp<uint32_t>(_window.get_width_px(), surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
	chosen_extent.height = std::clamp<uint32_t>(_window.get_height_px(), surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);
    } else {
	chosen_extent = surface_capabilities.currentExtent;
    }

    // TOOD: CONTINUE HERE



}

Graphics::Swap_chain::operator VkSwapchainKHR& () noexcept {
    return swap_chain;
}

Graphics::Swap_chain::~Swap_chain() {

}
