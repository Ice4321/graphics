#include"graphics/devices/device_queue.hpp"
#include"graphics/devices/logical_device.hpp"

Graphics::Device_queue::Device_queue(Logical_device& _logical_device, std::uint32_t _family_index, std::uint32_t _index):
    family_index(_family_index), index(_index)
{ 
    VkQueue queue;
    vkGetDeviceQueue(_logical_device, family_index, index, &queue);
    // TODO: Is this safe? *this is passed to the function before its lifetime begins
    // Why are template parameters not needed after the second Unique_handle?
    Unique_handle::operator=(Unique_handle(queue));
}

std::uint32_t Graphics::Device_queue::get_index() const noexcept {
    return index;
}

std::uint32_t Graphics::Device_queue::get_family_index() const noexcept {
    return family_index;
}

VkDeviceQueueCreateInfo Graphics::Device_queue::make_creation_info(std::uint32_t _family_index, std::uint32_t _count) {
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
