#include"graphics/physical_device.hpp"


std::vector<Graphics::Physical_device> Graphics::Physical_device::enumerate_all(VkInstance _instance) {
    std::vector<VkPhysicalDevice> devices;
    uint32_t device_count = 0;
    vkEnueratePhysicalDevices(_instance, &device_count, nullptr);
    devices.resize(device_count);
    vkEnueratePhysicalDevices(_instance, &device_count, devices.data());


}


Graphics::Physical_device::operator VkPhysicalDevice& () noexcept {
    return physical_device;
}


Graphics::Physical_device::Physical_device(VkPhysicalDevice _physical_device) noexcept: 
    physical_device(_physical_device)
{ }
