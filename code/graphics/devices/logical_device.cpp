#include"graphics/devices/logical_device.hpp"
#include<set>
#include<array>
#include<ranges>
#include<algorithm>

Graphics::Logical_device::Logical_device(Physical_device& _physical_device, Surface& _surface) {
    std::vector<VkDeviceQueueCreateInfo> queue_creation_info;
    std::uint32_t graphics_queue_family_index, presentation_queue_family_index;

    // TODO: Check if this is correct!
    
    {
	auto has_graphics_support = [&](std::uint32_t _index) { return _physical_device.get_queue_family_properties(_index).queueFlags & VK_QUEUE_GRAPHICS_BIT; };
	auto has_presentation_support = [&](std::uint32_t _index) { return _surface.test_queue_family_presentation_support(_physical_device, _index); };
	auto indices = std::ranges::iota_view<std::uint32_t, std::uint32_t>(0, _physical_device.get_queue_family_count());

	auto common = indices | std::views::filter(has_graphics_support) | std::views::filter(has_presentation_support);

	// TODO: can operator* be used with an r-value iterator?
	if(!common.empty()) {
	    queue_creation_info.emplace_back(Device_queue::make_creation_info(*std::ranges::begin(common), 1));
	    graphics_queue_family_index = *std::ranges::begin(common);
	    presentation_queue_family_index = *std::ranges::begin(common);
	} else {
	    auto graphics = indices | std::views::filter(has_graphics_support);
	    auto presentation = indices | std::views::filter(has_presentation_support);
	    assert(!graphics.empty() && !presentation.empty());
	    queue_creation_info.emplace_back(Device_queue::make_creation_info(*std::ranges::begin(graphics), 1));
	    queue_creation_info.emplace_back(Device_queue::make_creation_info(*std::ranges::begin(presentation), 1));
	    graphics_queue_family_index = *std::ranges::begin(graphics);
	    presentation_queue_family_index = *std::ranges::begin(presentation);
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
    assert(vkCreateDevice(_physical_device, &logical_device_create_info, nullptr, &logical_device) == VK_SUCCESS); 
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






