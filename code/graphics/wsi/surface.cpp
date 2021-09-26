#include "graphics/wsi/surface.hpp"
#include "graphics/wsi/window.hpp"
#include "graphics/state/globals.hpp"

Graphics::Surface::Surface(Window& _window) {
    Handle surface;
    VULKAN_ASSERT(glfwCreateWindowSurface(*instance, _window, nullptr, &surface)); 
    Unique_handle::operator=({surface, [](Handle _surface) { vkDestroySurfaceKHR(*instance, _surface, nullptr); }});
}


