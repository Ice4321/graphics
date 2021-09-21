#include"graphics/devices/physical_device.hpp"
#include<cstdint>
#include "graphics/utility/vulkan_assert.hpp"

std::vector<Graphics::Physical_device> Graphics::Physical_device::enumerate_all(Instance& _instance) {
    std::uint32_t device_count = 0;
    VULKAN_ASSERT(vkEnumeratePhysicalDevices(_instance, &device_count, nullptr));
    std::vector<VkPhysicalDevice> devices(device_count);
    VULKAN_ASSERT(vkEnumeratePhysicalDevices(_instance, &device_count, devices.data()));

    return { std::begin(devices), std::end(devices) };
}

Graphics::Physical_device::Physical_device(VkPhysicalDevice _physical_device): 
    Unique_handle<VkPhysicalDevice>(_physical_device)
{ 
    vkGetPhysicalDeviceProperties(*this, &properties);

    std::uint32_t queue_family_properties_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(*this, &queue_family_properties_count, nullptr);
    queue_family_properties.resize(queue_family_properties_count);
    vkGetPhysicalDeviceQueueFamilyProperties(*this, &queue_family_properties_count, queue_family_properties.data());

}

VkPhysicalDeviceProperties const& Graphics::Physical_device::get_properties() const noexcept {
    return properties;
}

std::uint32_t Graphics::Physical_device::get_queue_family_count() const noexcept {
    return queue_family_properties.size();
}

VkQueueFamilyProperties const& Graphics::Physical_device::get_queue_family_properties(std::uint32_t _queue_index) const {
    return queue_family_properties[_queue_index];
}

