#include "graphics/devices/physical_device.hpp"
#include "graphics/utility/vulkan_assert.hpp"
#include "graphics/instance/instance.hpp"
#include "graphics/wsi/surface.hpp"
#include <cstdint>

std::vector<Graphics::Physical_device> Graphics::Physical_device::enumerate_all(Instance& _instance) {
    std::uint32_t device_count = 0;
    VULKAN_ASSERT(vkEnumeratePhysicalDevices(_instance, &device_count, nullptr));
    std::vector<VkPhysicalDevice> devices(device_count);
    VULKAN_ASSERT(vkEnumeratePhysicalDevices(_instance, &device_count, devices.data()));

    return { std::begin(devices), std::end(devices) };
}

Graphics::Physical_device::Physical_device(Handle _physical_device): 
    Unique_handle(_physical_device)
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

bool Graphics::Physical_device::get_queue_family_presentation_support(Surface& _surface, std::uint32_t _queue_family_index) {
    VkBool32 result;
    VULKAN_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(*this, _queue_family_index, _surface, &result));
    return result;
}

VkSurfaceCapabilitiesKHR Graphics::Physical_device::get_surface_capabilities(Surface& _surface) {
    VkSurfaceCapabilitiesKHR surface_capabilities;
    VULKAN_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*this, _surface, &surface_capabilities));
    return surface_capabilities;
}

std::vector<VkSurfaceFormatKHR> Graphics::Physical_device::get_surface_formats(Surface& _surface) {
    std::uint32_t surface_format_count;
    VULKAN_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(*this, _surface, &surface_format_count, nullptr));
    std::vector<VkSurfaceFormatKHR> surface_formats(surface_format_count);
    VULKAN_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(*this, _surface, &surface_format_count, surface_formats.data()));
    return surface_formats;
}

std::vector<VkPresentModeKHR> Graphics::Physical_device::get_surface_presentation_modes(Surface& _surface) {
    std::uint32_t surface_presentation_mode_count;
    VULKAN_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(*this, _surface, &surface_presentation_mode_count, nullptr));
    std::vector<VkPresentModeKHR> surface_presentation_modes(surface_presentation_mode_count);
    VULKAN_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(*this, _surface, &surface_presentation_mode_count, surface_presentation_modes.data()));
    return surface_presentation_modes;
}


