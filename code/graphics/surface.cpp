#include"graphics/surface.hpp"
#include<GLFW/glfw3.h>
#include"graphics/utility/vulkan_assert.hpp"

Graphics::Surface::Surface(Instance& _instance, Window& _window):
    instance(&_instance)
{
    VULKAN_ASSERT(glfwCreateWindowSurface(*instance, _window, nullptr, &surface)); 
}

Graphics::Surface::operator VkSurfaceKHR& () noexcept {
    return surface;
}

Graphics::Surface::~Surface() {
    vkDestroySurfaceKHR(*instance, surface, nullptr);
}

bool Graphics::Surface::test_queue_family_presentation_support(Physical_device& _physical_device, std::uint32_t _queue_family_index) {
    VkBool32 result;
    VULKAN_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(_physical_device, _queue_family_index, surface, &result));
    return result;
}
