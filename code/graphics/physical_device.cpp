#include"graphics/physical_device.hpp"
#include<ranges>

std::vector<Graphics::Physical_device> Graphics::Physical_device::enumerate_all(VkInstance _instance) {
    std::vector<VkPhysicalDevice> devices;

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(_instance, &device_count, nullptr);
    devices.resize(device_count);
    vkEnumeratePhysicalDevices(_instance, &device_count, devices.data());

    // TODO: finish this
    
    // Necessary because the constructor is private
    //auto transform = std::views::transform(devices, [](VkPhysicalDevice _device) -> Physical_device { return _device; });

    return { std::ranges::cbegin(devices), std::ranges::cend(devices) };
}


Graphics::Physical_device::operator VkPhysicalDevice& () noexcept {
    return physical_device;
}


Graphics::Physical_device::Physical_device(VkPhysicalDevice _physical_device) noexcept: 
    physical_device(_physical_device)
{ }
