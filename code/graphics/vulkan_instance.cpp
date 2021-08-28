#include"graphics/vulkan_instance.hpp"
#include"utility/critical_error.hpp"
#include"graphics/window.hpp"
#include<array>
#include<iostream>

Graphics::Vulkan_instance::Vulkan_instance(Use_validation_layers _use_validation_layers):
    validation_enabled(_use_validation_layers.value) 
{
    // TODO: replace _use_validation_layers with validation_enabled
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
    if(_use_validation_layers.value) {
	all_extensions.emplace_back(+VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
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
	.pfnUserCallback = &debug_messenger_callback,
	.pUserData = nullptr
    };

    VkInstanceCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	.pNext = &debug_messenger_create_info,
	.flags = 0,
	.pApplicationInfo = &application_info,
	.enabledLayerCount = uint32_t(_use_validation_layers.value ? validation_layers.size() : 0),
	.ppEnabledLayerNames = _use_validation_layers.value ? validation_layers.data() : nullptr,
	.enabledExtensionCount = (uint32_t)all_extensions.size(),
	.ppEnabledExtensionNames = all_extensions.data()
    };


    if(vkCreateInstance(&create_info, nullptr, &instance) < 0) critical_error("vkCreateInstance()");

    if(_use_validation_layers.value) {
	// TODO: skip void_func... and cast immediately if safe
	PFN_vkVoidFunction void_func_vkCreateDebugUtilsMessengerEXT = vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if(!void_func_vkCreateDebugUtilsMessengerEXT) critical_error("vkGetInstanceProcAddr(vkCreateDebugUtilsMessengerEXT)");
	PFN_vkCreateDebugUtilsMessengerEXT func_vkCreateDebugUtilsMessengerEXT = 
	    reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(void_func_vkCreateDebugUtilsMessengerEXT);

	if(func_vkCreateDebugUtilsMessengerEXT(instance, &debug_messenger_create_info, nullptr, &debug_messenger) < 0) 
	    critical_error("vkCreateDebugUtilsMessengerEXT()");

    }
}

Graphics::Vulkan_instance::operator VkInstance& () noexcept {
    return instance;
}

Graphics::Vulkan_instance::~Vulkan_instance() {
    PFN_vkVoidFunction void_func_vkDestroyDebugUtilsMessengerEXT = vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if(!void_func_vkDestroyDebugUtilsMessengerEXT) critical_error("vkGetInstanceProcAddr(vkDestroyDebugUtilsMessengerEXT)");
    PFN_vkDestroyDebugUtilsMessengerEXT func_vkDestroyDebugUtilsMessengerEXT = 
	reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(void_func_vkDestroyDebugUtilsMessengerEXT);

    func_vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
    vkDestroyInstance(instance, nullptr);
}


VKAPI_ATTR VkBool32 VKAPI_CALL Graphics::Vulkan_instance::debug_messenger_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
    VkDebugUtilsMessageTypeFlagsEXT _message_types,
    VkDebugUtilsMessengerCallbackDataEXT const* _callback_data,
    void* _user_data
) {
    // TODO: make this better
    std::cout << "Vulkan: " << _callback_data->pMessage << std::endl;

    (void)_message_severity;
    (void)_message_types;
    (void)_user_data;
    
    // Must always return false
    return VK_FALSE;
}
