#include "graphics/commands/queue.hpp"
#include "graphics/devices/logical_device.hpp"
#include "preprocessor/macros.hpp"

Graphics::Queue::Queue(Logical_device* _logical_device, std::uint32_t _family_index, std::uint32_t _index):
    family_index(_family_index), index(_index)
{ 
    VkQueue queue;
    vkGetDeviceQueue(*_logical_device, family_index, index, &queue);
    Unique_handle::operator=({queue});
}

std::uint32_t Graphics::Queue::get_index() const noexcept {
    IF_DEBUG(ASSERT(has_value()));
    return index;
}

std::uint32_t Graphics::Queue::get_family_index() const noexcept {
    IF_DEBUG(ASSERT(has_value()));
    return family_index;
}

VkDeviceQueueCreateInfo Graphics::Queue::make_creation_info(std::uint32_t _family_index, std::uint32_t _count) {
    // The lifetime of this array must continue after exiting this function, because it's referenced in VkDeviceQueueCreateInfo
    static std::vector<float> const priorities(64, 1.0f);

    return {
	.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.queueFamilyIndex = _family_index,
	.queueCount = _count,
	.pQueuePriorities = priorities.data()
    };

}
