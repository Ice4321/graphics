#include"graphics/logical_device.hpp"
#include<array>


Graphics::Logical_device::Logical_device(Physical_device& _physical_device) {
    // Number of queues in the graphics family to create
    constexpr uint32_t graphics_queue_count = 1;
    std::array<float, graphics_queue_count> graphics_queue_priorities = { 1.0f };
    
    // TODO: Is this zeroed out properly??
    //std::array<VkDeviceQueueCreateInfo, 1> all_queues_create_info{};
    std::array<VkDeviceQueueCreateInfo, 1> all_queues_create_info{{{
	.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.queueFamilyIndex = (uint32_t)_physical_device.get_queue_family_index(VK_QUEUE_GRAPHICS_BIT),
	.queueCount = graphics_queue_count,
	.pQueuePriorities = graphics_queue_priorities.data()
    }}};

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

}

Graphics::Logical_device::~Logical_device() {
    // TODO
}
