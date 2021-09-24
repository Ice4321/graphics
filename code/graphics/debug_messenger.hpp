#pragma once

#include "utility/unique_handle.hpp"
#include "events/emitter.hpp"
#include <vulkan/vulkan.h>

namespace Events::Graphics::Debug_messenger {
    struct Message {

    };
}

namespace Graphics {
    class Debug_messenger final: public Utility::Unique_handle<VkDebugUtilsMessengerEXT>,
				 public Event_emitter<
				    Events::Graphics::Debug_messenger::Message
				 >
{
    public:
	static VKAPI_ATTR VkBool32 VKAPI_CALL global_callback(
	    VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
	    VkDebugUtilsMessageTypeFlagsEXT _message_types,
	    VkDebugUtilsMessengerCallbackDataEXT const* _callback_data,
	    void* _user_data
	);

	Debug_messenger();
	void initialise(class Instance* _instance);

	VkDebugUtilsMessengerCreateInfoEXT const& get_creation_info() const noexcept;

	
    private:
	class Instance* instance;
	VkDebugUtilsMessengerCreateInfoEXT creation_info;

    };
}
