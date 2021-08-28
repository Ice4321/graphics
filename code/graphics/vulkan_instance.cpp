#include"graphics/vulkan_instance.hpp"
#include"utility/critical_error.hpp"
#include"graphics/window.hpp"

Graphics::Vulkan_instance::Vulkan_instance() {
    VkApplicationInfo application_info{
	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	.pNext = nullptr,
	.pApplicationName = nullptr,
	.applicationVersion = 0,
	.pEngineName = nullptr,
	.engineVersion = 0,
	.apiVersion = VK_API_VERSION_1_2
    };

    auto [glfw_extensions, glfw_extension_count] = Window::get_required_instance_extensions();

    VkInstanceCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.pApplicationInfo = &application_info,
	.enabledLayerCount = 0,
	.ppEnabledLayerNames = nullptr,
	.enabledExtensionCount = glfw_extension_count,
	.ppEnabledExtensionNames = glfw_extensions
    };

    if(vkCreateInstance(&create_info, nullptr, &instance) < 0) critical_error("vkCreateInstance()");
}

Graphics::Vulkan_instance::operator VkInstance& () noexcept {
    return instance;
}

Graphics::Vulkan_instance::~Vulkan_instance() {
    vkDestroyInstance(instance, nullptr);
}
