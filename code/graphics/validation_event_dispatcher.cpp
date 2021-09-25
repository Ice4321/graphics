#include "graphics/validation_event_dispatcher.hpp"
#include "graphics/utility/vulkan_assert.hpp"


VKAPI_ATTR VkBool32 VKAPI_CALL Graphics::Validation_event_dispatcher::forward(
	VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
	VkDebugUtilsMessageTypeFlagsEXT _message_types,
	VkDebugUtilsMessengerCallbackDataEXT const* _callback_data,
	void* _user_data
) {
    Events::Graphics::Validation_event_dispatcher::Message event(_callback_data->pMessage);
    static_cast<Validation_event_dispatcher*>(_user_data)->post_event(std::as_const(event));

    // Must always return false
    return VK_FALSE;
}

Graphics::Validation_event_dispatcher::Validation_event_dispatcher():
    debug_messenger_creation_info{
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
	.pfnUserCallback = &Validation_event_dispatcher::forward,
	.pUserData = static_cast<void*>(this)
    }
{ }

VkDebugUtilsMessengerCreateInfoEXT const& Graphics::Validation_event_dispatcher::get_debug_messenger_creation_info() {
    return debug_messenger_creation_info;
}
