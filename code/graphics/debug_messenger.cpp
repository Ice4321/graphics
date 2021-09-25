#include "graphics/debug_messenger.hpp"
#include "graphics/instance.hpp"
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Debug_messenger::Debug_messenger():
    instance(nullptr),
    creation_info{
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
	.pfnUserCallback = &Debug_messenger::global_callback,
	.pUserData = static_cast<void*>(this)
    }
{ }

void Graphics::Debug_messenger::initialise(Instance* _instance) {
    instance = _instance;

    VkDebugUtilsMessengerEXT debug_messenger;
    auto ptr_vkCreateDebugUtilsMessengerEXT = instance->get_function_address<PFN_vkCreateDebugUtilsMessengerEXT>("vkCreateDebugUtilsMessengerEXT");
    VULKAN_ASSERT(ptr_vkCreateDebugUtilsMessengerEXT(*instance, &creation_info, nullptr, &debug_messenger));

    Unique_handle::operator=({ 
	debug_messenger, 
	[_instance](Handle _debug_messenger) { 
	    auto ptr_vkDestroyDebugUtilsMessengerEXT = _instance->get_function_address<PFN_vkDestroyDebugUtilsMessengerEXT>("vkDestroyDebugUtilsMessengerEXT");
	    ptr_vkDestroyDebugUtilsMessengerEXT(*_instance, _debug_messenger, nullptr);
	}
    });
}

void Graphics::Debug_messenger::destroy() {
    Unique_handle::operator=({});
}

VkDebugUtilsMessengerCreateInfoEXT const& Graphics::Debug_messenger::get_creation_info() const noexcept{
    return creation_info;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Graphics::Debug_messenger::global_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT _message_severity,
	VkDebugUtilsMessageTypeFlagsEXT _message_types,
	VkDebugUtilsMessengerCallbackDataEXT const* _callback_data,
	void* _user_data
) {
    // The callback must not make calls to any Vulkan commands (VUID-PFN_vkDebugUtilsMessengerCallbackEXT-None-04769)
    
    Events::Graphics::Debug_messenger::Message event(_callback_data->pMessage);
    static_cast<Debug_messenger*>(_user_data)->post_event(std::as_const(event));

    // Must always return false
    return VK_FALSE;
}

