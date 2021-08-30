#include"graphics/logical_device.hpp"
#include"utility/critical_error.hpp"
#include<set>
#include<array>

Graphics::Logical_device::Logical_device(Physical_device& _physical_device, Surface& _surface) {
    std::vector<VkDeviceQueueCreateInfo> queue_creation_info = create_queue_creation_info(_physical_device, _surface);

    std::array extensions{
	+VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo logical_device_create_info{
	.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.queueCreateInfoCount = (uint32_t)queue_creation_info.size(),
	.pQueueCreateInfos = queue_creation_info.data(),
	.enabledLayerCount = 0,
	.ppEnabledLayerNames = nullptr,
	.enabledExtensionCount = extensions.size(),
	.ppEnabledExtensionNames = extensions.data(),
	.pEnabledFeatures = nullptr
    };

    if(vkCreateDevice(_physical_device, &logical_device_create_info, nullptr, &logical_device) < 0) critical_error("vkCreateDevice()");
    
    vkGetDeviceQueue(logical_device, graphics_queue_family_index, 0, &graphics_queue_handle);
    vkGetDeviceQueue(logical_device, presentation_queue_family_index, 0, &presentation_queue_handle);
}

Graphics::Logical_device::operator VkDevice& () noexcept {
    return logical_device;
}


VkQueue Graphics::Logical_device::get_graphics_queue() noexcept {
    return graphics_queue_handle;
}

Graphics::Logical_device::~Logical_device() {
    vkDestroyDevice(logical_device, nullptr);
}


std::vector<VkDeviceQueueCreateInfo> Graphics::Logical_device::create_queue_creation_info(Physical_device& _physical_device, Surface& _surface) {
    static auto check_queue_family_presentation_support = [](Physical_device& _physical_device, std::size_t _queue_family_index, Surface& _surface) -> bool {
	VkBool32 result;
	if(vkGetPhysicalDeviceSurfaceSupportKHR(_physical_device, _queue_family_index, _surface, &result) < 0) {
	    critical_error("vkGetPhysicalDeviceSurfaceSupportKHR()");
	}
	return result;
    };
    
    // The lifetime of this array must continue after exiting this function, because it's referenced in VkDeviceQueueCreateInfo
    static float priorities[] = { 1.0f };

    std::vector<VkQueueFamilyProperties> const& queue_family_properties = _physical_device.get_queue_family_properties();
    
    // The index corresponds to the index of an element of the pQueueFamilyProperties array 
    // that was returned by vkGetPhysicalDeviceQueueFamilyProperties()
    graphics_queue_family_index = -1; presentation_queue_family_index = -1;

    for(std::size_t index = 0; index < queue_family_properties.size(); ++index) {
	bool has_graphics_support = queue_family_properties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT;
	bool has_presentation_support = check_queue_family_presentation_support(_physical_device, index, _surface);
	
	// Prefer to use the same queue for graphics and presentation
	if(has_graphics_support && has_presentation_support) {
	    graphics_queue_family_index = index;
	    presentation_queue_family_index = index;
	    break;
	} else {
	    if(has_graphics_support && graphics_queue_family_index == -1) graphics_queue_family_index = index;
	    if(has_presentation_support && presentation_queue_family_index == -1) presentation_queue_family_index = index;
	}
    }
    
    if(graphics_queue_family_index == -1 || presentation_queue_family_index == -1) critical_error("No suitable device queues");

    std::set<int> unique_queue_family_indices({graphics_queue_family_index, presentation_queue_family_index});

    std::vector<VkDeviceQueueCreateInfo> queue_creation_info;
    for(auto index : unique_queue_family_indices) {
	queue_creation_info.emplace_back(
	    // Create one queue in each familiy
	    VkDeviceQueueCreateInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueFamilyIndex = (uint32_t)index,
		.queueCount = 1,
		.pQueuePriorities = priorities
	    }
	);
    }

    return queue_creation_info;
}






