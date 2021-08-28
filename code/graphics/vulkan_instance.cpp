#include"graphics/vulkan_instance.hpp"
#include"utility/critical_error.hpp"
#include"graphics/window.hpp"
#include<array>

Graphics::Vulkan_instance::Vulkan_instance(Use_validation_layers _use_validation_layers) {
    VkApplicationInfo application_info{
	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	.pNext = nullptr,
	.pApplicationName = nullptr,
	.applicationVersion = 0,
	.pEngineName = nullptr,
	.engineVersion = 0,
	.apiVersion = VK_API_VERSION_1_2
    };
    
    std::array validation_layers{
	+"VK_LAYER_KHRONOS_validation"
    };
    
    // TODO: Unary plus necessary? Does it apply array to pointer conversions?
    std::vector<char const*> all_extensions;
    auto [glfw_extensions, glfw_extension_count] = Window::get_required_instance_extensions();
    for(std::size_t i = 0; i < glfw_extension_count; ++i) all_extensions.emplace_back(glfw_extensions[i]);
    if(_use_validation_layers.value) {
	all_extensions.emplace_back(+VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    VkInstanceCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.pApplicationInfo = &application_info,
	.enabledLayerCount = uint32_t(_use_validation_layers.value ? validation_layers.size() : 0),
	.ppEnabledLayerNames = _use_validation_layers.value ? validation_layers.data() : nullptr,
	.enabledExtensionCount = (uint32_t)all_extensions.size(),
	.ppEnabledExtensionNames = all_extensions.data()
    };

    if(vkCreateInstance(&create_info, nullptr, &instance) < 0) critical_error("vkCreateInstance()");
}

Graphics::Vulkan_instance::operator VkInstance& () noexcept {
    return instance;
}

Graphics::Vulkan_instance::~Vulkan_instance() {
    vkDestroyInstance(instance, nullptr);
}
