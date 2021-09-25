#pragma once

#include "utility/unique_handle.hpp"
#include "events/emitter.hpp"
#include <vulkan/vulkan.h>

namespace Events::Graphics::Validation_event_dispatcher {
    struct Message {
	char const* text;
    };
}

// Event handlers must not make calls to any Vulkan commands (VUID-PFN_vkDebugUtilsMessengerCallbackEXT-None-04769)
 
namespace Graphics {
    class Validation_event_dispatcher final: 
	public Event_emitter<
	    Events::Graphics::Validation_event_dispatcher::Message
	 >
{
    public:
	// Called by Vulkan, forwards the messages (as events) to associated Validation_event_dispatcher objects
	static VKAPI_ATTR VkBool32 VKAPI_CALL forward(
	    VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
	    VkDebugUtilsMessageTypeFlagsEXT _message_types,
	    VkDebugUtilsMessengerCallbackDataEXT const* _callback_data,
	    void* _user_data
	);

	Validation_event_dispatcher();
	
	// Needed for constructing VkInstance and Debug_messenger objects associated with this Validation_event_dispatcher object
	VkDebugUtilsMessengerCreateInfoEXT const& get_debug_messenger_creation_info();
	
    private:
	VkDebugUtilsMessengerCreateInfoEXT debug_messenger_creation_info;

    };
}
