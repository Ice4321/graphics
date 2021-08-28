#ifndef INCLUDED_GRAPHICS_VULKAN_INSTANCE_HPP
#define INCLUDED_GRAPHICS_VULKAN_INSTANCE_HPP

#include<vulkan/vulkan.h>

// TODO: rename this and files to instance instead of vulkan_instance
namespace Graphics {
    class Vulkan_instance {
    public:
	struct Use_validation_layers { bool value; };

	Vulkan_instance(Use_validation_layers _use_validation_layers);

	operator VkInstance& () noexcept;

	~Vulkan_instance();

	Vulkan_instance(Vulkan_instance const&) = delete;
	Vulkan_instance& operator=(Vulkan_instance const&) = delete;

    private:
	bool validation_enabled;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debug_messenger;
	
	// TODO: remove this and replace with a debug messenger class
	// The callback must not make calls to any Vulkan commands (VUID-PFN_vkDebugUtilsMessengerCallbackEXT-None-04769)
	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_messenger_callback(
	    VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
	    VkDebugUtilsMessageTypeFlagsEXT _message_types,
	    VkDebugUtilsMessengerCallbackDataEXT const* _callback_data,
	    void* _user_data
	);

    };
}

#endif
