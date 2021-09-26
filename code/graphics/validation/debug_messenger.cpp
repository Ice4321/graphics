#include "graphics/validation/debug_messenger.hpp"
#include "graphics/validation/message_dispatcher.hpp"
#include "graphics/state/globals.hpp"
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Debug_messenger::Debug_messenger(Validation_event_dispatcher& _validation_event_dispatcher) {
    Handle debug_messenger;
    auto ptr_vkCreateDebugUtilsMessengerEXT = instance->get_function_address<PFN_vkCreateDebugUtilsMessengerEXT>("vkCreateDebugUtilsMessengerEXT");
    VULKAN_ASSERT(ptr_vkCreateDebugUtilsMessengerEXT(
	*instance, &_validation_event_dispatcher.get_debug_messenger_creation_info(), nullptr, &debug_messenger
    ));

    Unique_handle::operator=({ 
	debug_messenger, 
	[](Handle _debug_messenger) { 
	    auto ptr_vkDestroyDebugUtilsMessengerEXT = instance->get_function_address<PFN_vkDestroyDebugUtilsMessengerEXT>("vkDestroyDebugUtilsMessengerEXT");
	    ptr_vkDestroyDebugUtilsMessengerEXT(*instance, _debug_messenger, nullptr);
	}
    });
}
