#include"graphics/surface.hpp"
#include<GLFW/glfw3.h>

Graphics::Surface::Surface(Instance& _instance, Window& _window):
    instance(_instance)
{
    if(glfwCreateWindowSurface(instance, _window, nullptr, &surface) < 0) critical_error("glfwCreateWindowSurface()");
}

Graphics::Surface::~Surface() {
    vkDestroySurfaceKHR(instance, surface, nullptr);
}
