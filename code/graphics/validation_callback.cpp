#include"graphics/validation_callback.hpp"
#include<iostream>

VKAPI_ATTR VkBool32 VKAPI_CALL Graphics::Validation_callback::dispatch(
	VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
	VkDebugUtilsMessageTypeFlagsEXT _message_types,
	VkDebugUtilsMessengerCallbackDataEXT const* _callback_data,
	void* _user_data
) {
    static_cast<Validation_callback*>(_user_data)->execute(_message_severity, _message_types, _callback_data);

    // Must always return false
    return VK_FALSE;
}


void Graphics::Validation_callback::execute(
	VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
	VkDebugUtilsMessageTypeFlagsEXT _message_types,
	VkDebugUtilsMessengerCallbackDataEXT const* _callback_data
) {
    std::cout << "Vulkan: " << _callback_data->pMessage << std::endl;
}
