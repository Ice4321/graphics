#include"graphics/physical_device.hpp"
#include<ranges>
#include<algorithm>
#include"utility/critical_error.hpp"

std::vector<Graphics::Physical_device> Graphics::Physical_device::enumerate_all(VkInstance _instance) {
    std::vector<VkPhysicalDevice> devices;
    
    uint32_t device_count = 0;
    if(vkEnumeratePhysicalDevices(_instance, &device_count, nullptr) < 0) critical_error("vkEnumeratePhysicalDevices()");
    devices.resize(device_count);
    if(vkEnumeratePhysicalDevices(_instance, &device_count, devices.data()) < 0) critical_error("vkEnumeratePhysicalDevices()");

    return { std::begin(devices), std::end(devices) };
}


Graphics::Physical_device::operator VkPhysicalDevice& () noexcept {
    return physical_device;
}

VkPhysicalDeviceProperties const& Graphics::Physical_device::get_properties() const noexcept {
    return properties;
}

std::size_t Graphics::Physical_device::get_queue_family_index(VkQueueFlagBits _capabilities) const {
    auto found = std::ranges::find_if(
	queue_family_properties,
	[&](VkQueueFamilyProperties const& _x) { return _x.queueFlags & _capabilities; }
    );

    if(found == std::ranges::end(queue_family_properties)) throw Exceptions::Graphics::Physical_device::Queue_family_not_found{};

    // Index must correspond to the index in the array returned by vkGetPhysicalDeviceQueueFamilyProperties()
    return std::distance(std::ranges::begin(queue_family_properties), found);
}

Graphics::Physical_device::Physical_device(VkPhysicalDevice _physical_device) noexcept: 
    physical_device(_physical_device)
{ 
    vkGetPhysicalDeviceProperties(physical_device, &properties);

    uint32_t queue_family_properties_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_properties_count, nullptr);
    queue_family_properties.resize(queue_family_properties_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_properties_count, queue_family_properties.data());

}
