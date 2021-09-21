#include"graphics/devices/logical_device.hpp"
#include<set>
#include<array>
#include<ranges>
#include<algorithm>
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Logical_device::Logical_device(Physical_device& _physical_device, Surface& _surface) {
    std::vector<VkDeviceQueueCreateInfo> queue_creation_info;
    std::uint32_t graphics_queue_family_index, presentation_queue_family_index;

    {
	auto has_graphics_support = [&](std::uint32_t _index) { return _physical_device.get_queue_family_properties(_index).queueFlags & VK_QUEUE_GRAPHICS_BIT; };
	auto has_presentation_support = [&](std::uint32_t _index) { return _surface.test_queue_family_presentation_support(_physical_device, _index); };
	auto queue_family_indices = std::ranges::iota_view<std::uint32_t, std::uint32_t>(0, _physical_device.get_queue_family_count());

	auto with_both_support = queue_family_indices | std::views::filter(has_graphics_support) | std::views::filter(has_presentation_support);

	// TODO: can operator* be used with an r-value iterator?
	if(!with_both_support.empty()) {
	    queue_creation_info.emplace_back(Device_queue::make_creation_info(*std::ranges::begin(with_both_support), 1));
	    graphics_queue_family_index = *std::ranges::begin(with_both_support);
	    presentation_queue_family_index = *std::ranges::begin(with_both_support);
	} else {
	    auto with_graphics_support = queue_family_indices | std::views::filter(has_graphics_support);
	    auto with_presentation_support = queue_family_indices | std::views::filter(has_presentation_support);
	    ASSERT(!with_graphics_support.empty() && !with_presentation_support.empty());

	    queue_creation_info.emplace_back(Device_queue::make_creation_info(*std::ranges::begin(with_graphics_support ), 1));
	    queue_creation_info.emplace_back(Device_queue::make_creation_info(*std::ranges::begin(with_presentation_support ), 1));
	    graphics_queue_family_index = *std::ranges::begin(with_graphics_support);
	    presentation_queue_family_index = *std::ranges::begin(with_presentation_support);
	}
    }


    std::array extensions{
	+VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo logical_device_create_info{
	.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.queueCreateInfoCount = (std::uint32_t)queue_creation_info.size(),
	.pQueueCreateInfos = queue_creation_info.data(),
	.enabledLayerCount = 0,
	.ppEnabledLayerNames = nullptr,
	.enabledExtensionCount = extensions.size(),
	.ppEnabledExtensionNames = extensions.data(),
	.pEnabledFeatures = nullptr
    };
    
    VkDevice logical_device;
    VULKAN_ASSERT(vkCreateDevice(_physical_device, &logical_device_create_info, nullptr, &logical_device)); 
    Unique_handle::operator=(Unique_handle(logical_device, [](VkDevice _logical_device) { vkDestroyDevice(_logical_device, nullptr); }));
    
    // TODO: Calling Device_queue's ctor accesses this outside of lifetime. Is this UB? Shouldn't be because it's accessed through the this pointer
    graphics_queue = Device_queue(*this, graphics_queue_family_index, 0);
    presentation_queue = Device_queue(*this, presentation_queue_family_index, 0);
}


Graphics::Device_queue& Graphics::Logical_device::get_graphics_queue() noexcept {
    return graphics_queue;
}

Graphics::Device_queue& Graphics::Logical_device::get_presentation_queue() noexcept {
    return presentation_queue;
}

void Graphics::Logical_device::wait_idle() {
    vkDeviceWaitIdle(*this);
}






