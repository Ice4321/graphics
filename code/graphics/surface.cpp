#include"graphics/surface.hpp"
#include<GLFW/glfw3.h>

Graphics::Surface::Surface(Instance& _instance, Window& _window):
    instance(&_instance)
{
    assert(glfwCreateWindowSurface(*instance, _window, nullptr, &surface) == VK_SUCCESS); 
}

Graphics::Surface::operator VkSurfaceKHR& () noexcept {
    return surface;
}

Graphics::Surface::~Surface() {
    vkDestroySurfaceKHR(*instance, surface, nullptr);
}

bool Graphics::Surface::test_queue_family_presentation_support(Physical_device& _physical_device, std::uint32_t _queue_family_index) {
    VkBool32 result;
    assert(vkGetPhysicalDeviceSurfaceSupportKHR(_physical_device, _queue_family_index, surface, &result) == VK_SUCCESS);
    return result;
}
