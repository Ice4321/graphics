#pragma once

#include "utility/unique_handle.hpp"
#include "events/emitter.hpp"
#include <vulkan/vulkan.h>

// TODO: rename this file to message_dispatcher.hpp

namespace Events::Graphics::Validation_event_dispatcher {
    struct Message {
	// The values of VkDebugUtilsMessageSeverityFlagBitsEXT are sorted based on severity
	struct Severity { enum Value {
	    verbose = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
	    info = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
	    warning = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
	    error = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
	}; };
	struct Type_flags { enum Value {
	    general = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
	    validation = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
	    performance = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
	}; };

	Severity::Value severity;
	Type_flags::Value type_flags;
	char const* message_id_string; // Contains the portion of the Vulkan specification that has been violated
	std::uint32_t message_id; // Internal number associated with the message being triggered
	char const* message; // Detailed info
    };
}

// Event handlers must not make calls to any Vulkan commands (VUID-PFN_vkDebugUtilsMessengerCallbackEXT-None-04769)
 
namespace Graphics {
    class Validation_event_dispatcher: 
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
