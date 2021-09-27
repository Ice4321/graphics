#include "graphics/wsi/surface.hpp"
#include "graphics/wsi/window.hpp"
#include "graphics/state/instance.hpp"

Graphics::Surface::Surface(Instance& _instance, Window& _window) {
    Handle surface;
    VULKAN_ASSERT(glfwCreateWindowSurface(_instance, _window, nullptr, &surface)); 
    Unique_handle::operator=({surface, [&_instance](Handle _surface) { vkDestroySurfaceKHR(_instance, _surface, nullptr); }});
}


