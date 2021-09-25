#include "graphics/validation/debug_messenger.hpp"
#include "graphics/validation/validation_event_dispatcher.hpp"
#include "graphics/instance/instance.hpp"
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Debug_messenger::Debug_messenger(class Instance* _instance, class Validation_event_dispatcher& _validation_event_dispatcher):
    instance(_instance)
{
    VkDebugUtilsMessengerEXT debug_messenger;
    auto ptr_vkCreateDebugUtilsMessengerEXT = instance->get_function_address<PFN_vkCreateDebugUtilsMessengerEXT>("vkCreateDebugUtilsMessengerEXT");
    VULKAN_ASSERT(ptr_vkCreateDebugUtilsMessengerEXT(
	*instance, &_validation_event_dispatcher.get_debug_messenger_creation_info(), nullptr, &debug_messenger
    ));

    Unique_handle::operator=({ 
	debug_messenger, 
	[_instance](Handle _debug_messenger) { 
	    auto ptr_vkDestroyDebugUtilsMessengerEXT = _instance->get_function_address<PFN_vkDestroyDebugUtilsMessengerEXT>("vkDestroyDebugUtilsMessengerEXT");
	    ptr_vkDestroyDebugUtilsMessengerEXT(*_instance, _debug_messenger, nullptr);
	}
    });
}
