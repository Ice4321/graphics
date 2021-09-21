#include"graphics/instance.hpp"
#include"graphics/window.hpp"
#include<array>
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Instance::Instance(Validation _validation):
    validation_enabled(_validation == Validation::enabled)
{
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
    
    std::vector<char const*> all_extensions;
    auto [glfw_extensions, glfw_extension_count] = Window::get_required_instance_extensions();
    for(std::size_t i = 0; i < glfw_extension_count; ++i) all_extensions.emplace_back(glfw_extensions[i]);
    if(validation_enabled) {
	all_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    // To capture events that occur while creating or destroying an instance an application can link a VkDebugUtilsMessengerCreateInfoEXT structure 
    // to the pNext element of the VkInstanceCreateInfo structure given to vkCreateInstance.
    // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/chap50.html#_examples_10
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{
	.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
	.pNext = nullptr,
	.flags = 0,
	.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			   VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
	.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
	.pfnUserCallback = &Validation_callback::dispatch,
	.pUserData = static_cast<void*>(&validation_callback)
    };

    VkInstanceCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	.pNext = validation_enabled ? &debug_messenger_create_info : nullptr,
	.flags = 0,
	.pApplicationInfo = &application_info,
	.enabledLayerCount = std::uint32_t(validation_enabled ? validation_layers.size() : 0),
	.ppEnabledLayerNames = validation_enabled ? validation_layers.data() : nullptr,
	.enabledExtensionCount = (std::uint32_t)all_extensions.size(),
	.ppEnabledExtensionNames = all_extensions.data()
    };


    VULKAN_ASSERT(vkCreateInstance(&create_info, nullptr, &instance)); 

    if(validation_enabled) {
	auto ptr_vkCreateDebugUtilsMessengerEXT = get_function_address<PFN_vkCreateDebugUtilsMessengerEXT>("vkCreateDebugUtilsMessengerEXT");
	VULKAN_ASSERT(ptr_vkCreateDebugUtilsMessengerEXT(instance, &debug_messenger_create_info, nullptr, &debug_messenger));
    }
}

Graphics::Instance::operator VkInstance& () noexcept {
    return instance;
}

Graphics::Instance::~Instance() {
    if(validation_enabled) {
	auto ptr_vkDestroyDebugUtilsMessengerEXT = get_function_address<PFN_vkDestroyDebugUtilsMessengerEXT>("vkDestroyDebugUtilsMessengerEXT");
	ptr_vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
    }

    vkDestroyInstance(instance, nullptr);
}

