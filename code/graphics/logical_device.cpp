#include"graphics/logical_device.hpp"
#include"utility/critical_error.hpp"
#include<array>


Graphics::Logical_device::Logical_device(Physical_device& _physical_device) {
    uint32_t graphics_queue_family_index = 0;
    try {
	graphics_queue_family_index = (uint32_t)_physical_device.get_queue_family_index(VK_QUEUE_GRAPHICS_BIT);
    } catch(Exceptions::Graphics::Physical_device::Queue_family_not_found const&) {
	critical_error("Queue family not available");
    }
    // Number of queues in the graphics family to create
    constexpr uint32_t graphics_queue_count = 1;
    std::array<float, graphics_queue_count> graphics_queue_priorities = { 1.0f };
    
    std::array<VkDeviceQueueCreateInfo, 1> all_queues_create_info{{
	// Create a single graphics queue
	{
	    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	    .pNext = nullptr,
	    .flags = 0,
	    .queueFamilyIndex = graphics_queue_family_index,
	    .queueCount = graphics_queue_count,
	    .pQueuePriorities = graphics_queue_priorities.data()
	}
    }};

    VkDeviceCreateInfo logical_device_create_info{
	.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.queueCreateInfoCount = all_queues_create_info.size(),
	.pQueueCreateInfos = all_queues_create_info.data(),
	.enabledLayerCount = 0,
	.ppEnabledLayerNames = nullptr,
	.enabledExtensionCount = 0,
	.ppEnabledExtensionNames = nullptr,
	.pEnabledFeatures = nullptr
    };

    if(vkCreateDevice(_physical_device, &logical_device_create_info, nullptr, &logical_device) < 0) critical_error("vkCreateDevice()");
    
    // Retrieve the handle to the first (and only) queue in the graphics family
    vkGetDeviceQueue(logical_device, graphics_queue_family_index, 0, &graphics_queue_handle);
}

VkQueue Graphics::Logical_device::get_graphics_queue() noexcept {
    return graphics_queue_handle;
}

Graphics::Logical_device::~Logical_device() {
    vkDestroyDevice(logical_device, nullptr);
}
