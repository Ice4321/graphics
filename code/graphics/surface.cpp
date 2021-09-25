#include "graphics/wsi/surface.hpp"
#include "graphics/devices/physical_device.hpp"
#include "graphics/instance/instance.hpp"

Graphics::Surface::Surface(Instance* _instance, Window& _window):
    instance(_instance)
{
    VkSurfaceKHR surface;
    VULKAN_ASSERT(glfwCreateWindowSurface(*instance, _window, nullptr, &surface)); 
    Unique_handle::operator=({surface, [_instance](Handle _surface) { vkDestroySurfaceKHR(*_instance, _surface, nullptr); }
}

bool Graphics::Surface::test_queue_family_presentation_support(Physical_device& _physical_device, std::uint32_t _queue_family_index) {
    VkBool32 result;
    VULKAN_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(_physical_device, _queue_family_index, *this, &result));
    return result;
}
