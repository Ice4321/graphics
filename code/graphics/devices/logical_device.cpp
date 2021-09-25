#include "graphics/devices/logical_device.hpp"
#include "graphics/utility/vulkan_assert.hpp"
#include "graphics/devices/physical_device.hpp"
#include "graphics/wsi/surface.hpp"
#include <set>
#include <array>
#include <ranges>
#include <algorithm>

Graphics::Logical_device::Logical_device(Physical_device& _physical_device, Surface& _surface) {
    std::vector<VkDeviceQueueCreateInfo> queue_creation_info;
    std::uint32_t graphics_queue_family_index, presentation_queue_family_index;

    {
	auto has_graphics_support = [&](std::uint32_t _index) { return _physical_device.get_queue_family_properties(_index).queueFlags & VK_QUEUE_GRAPHICS_BIT; };
	auto has_presentation_support = [&](std::uint32_t _index) { return _surface.test_queue_family_presentation_support(_physical_device, _index); };
	auto queue_family_indices = std::ranges::iota_view<std::uint32_t, std::uint32_t>(0, _physical_device.get_queue_family_count());

	auto with_both_support = queue_family_indices | std::views::filter(has_graphics_support) | std::views::filter(has_presentation_support);

	if(!with_both_support.empty()) {
	    queue_creation_info.emplace_back(Queue::make_creation_info(*std::ranges::begin(with_both_support), 1));
	    graphics_queue_family_index = *std::ranges::begin(with_both_support);
	    presentation_queue_family_index = *std::ranges::begin(with_both_support);
	} else {
	    auto with_graphics_support = queue_family_indices | std::views::filter(has_graphics_support);
	    auto with_presentation_support = queue_family_indices | std::views::filter(has_presentation_support);
	    ASSERT(!with_graphics_support.empty() && !with_presentation_support.empty());

	    queue_creation_info.emplace_back(Queue::make_creation_info(*std::ranges::begin(with_graphics_support ), 1));
	    queue_creation_info.emplace_back(Queue::make_creation_info(*std::ranges::begin(with_presentation_support ), 1));
	    graphics_queue_family_index = *std::ranges::begin(with_graphics_support);
	    presentation_queue_family_index = *std::ranges::begin(with_presentation_support);
	}
    }


    std::array extensions{
	+VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo create_info{
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
    VULKAN_ASSERT(vkCreateDevice(_physical_device, &create_info, nullptr, &logical_device)); 
    Unique_handle::operator=({logical_device, [](Handle _logical_device) { vkDestroyDevice(_logical_device, nullptr); }});
    
    graphics_queue = {*this, graphics_queue_family_index, 0};
    presentation_queue = {*this, presentation_queue_family_index, 0};
}


Graphics::Queue const& Graphics::Logical_device::get_graphics_queue() const noexcept {
    return graphics_queue;
}

Graphics::Queue const& Graphics::Logical_device::get_presentation_queue() const noexcept {
    return presentation_queue;
}

void Graphics::Logical_device::submit_drawing_commands(Command_buffer& _command_buffer, Semaphore& _wait_sem, Semaphore& _signal_sem) {
    graphics_queue.submit(_command_buffer, _wait_sem, _signal_sem, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
}

void Graphics::Logical_device::present(Swap_chain& _swap_chain, std::uint32_t _swap_chain_image_index, Semaphore& _wait_sem) {
    presentation_queue.present(_swap_chain, _swap_chain_image_index, _wait_sem);
}

void Graphics::Logical_device::wait_idle() {
    vkDeviceWaitIdle(*this);
}






