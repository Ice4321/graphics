#ifndef INCLUDED_GRAPHICS_VALIDATION_CALLBACK_HPP
#define INCLUDED_GRAPHICS_VALIDATION_CALLBACK_HPP

#include<vulkan/vulkan.h>

namespace Graphics {
    // The callback must not make calls to any Vulkan commands (VUID-PFN_vkDebugUtilsMessengerCallbackEXT-None-04769)
    class Validation_callback {
    public:
	static VKAPI_ATTR VkBool32 VKAPI_CALL dispatch(
	    VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
	    VkDebugUtilsMessageTypeFlagsEXT _message_types,
	    VkDebugUtilsMessengerCallbackDataEXT const* _callback_data,
	    void* _user_data
	);

	Validation_callback() = default;

	void execute(
	    VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
	    VkDebugUtilsMessageTypeFlagsEXT _message_types,
	    VkDebugUtilsMessengerCallbackDataEXT const* _callback_data
	);

	~Validation_callback() = default;

	Validation_callback(Validation_callback const&) = delete;
	Validation_callback& operator=(Validation_callback const&) = delete;
    };
}

#endif
