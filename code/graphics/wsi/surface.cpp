#include "graphics/wsi/surface.hpp"
#include "graphics/wsi/window.hpp"
#include "graphics/devices/physical_device.hpp"
#include "graphics/instance/instance.hpp"

Graphics::Surface::Surface(Instance* _instance, Window& _window):
    instance(_instance)
{
    VkSurfaceKHR surface;
    VULKAN_ASSERT(glfwCreateWindowSurface(*instance, _window, nullptr, &surface)); 
    Unique_handle::operator=({surface, [_instance](Handle _surface) { vkDestroySurfaceKHR(*_instance, _surface, nullptr); }});
}


