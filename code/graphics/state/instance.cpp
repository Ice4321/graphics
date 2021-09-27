#include "graphics/state/instance.hpp"
#include "graphics/wsi/window.hpp"
#include <array>

Graphics::Instance::Instance(Validation _validation, std::function<void(Events::Graphics::Validation_event_dispatcher::Message const&)>&& _validation_message_callback):
    validation_enabled(_validation == Validation::enabled)
{
    VkApplicationInfo application_info{
	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	.pNext = nullptr,
	.pApplicationName = nullptr,
	.applicationVersion = 0,
	.pEngineName = nullptr,
	.engineVersion = 0,
	.apiVersion = VK_API_VERSION_1_2
    };
    
    std::array validation_layers{
	+"VK_LAYER_KHRONOS_validation"
    };
    
    std::vector<char const*> all_extensions;
    std::span<char const* const> glfw_extensions = Window::get_required_instance_extensions();
    for(char const* e : glfw_extensions) all_extensions.emplace_back(e);
    if(validation_enabled) all_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    validation_event_dispatcher.add_event_callback(std::move(_validation_message_callback));

    // To capture events that occur while creating or destroying an instance an application can link a VkDebugUtilsMessengerCreateInfoEXT structure 
    // to the pNext element of the VkInstanceCreateInfo structure given to vkCreateInstance.
    // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/chap50.html#_examples_10

    VkInstanceCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	.pNext = validation_enabled ? &validation_event_dispatcher.get_debug_messenger_creation_info() : nullptr,
	.flags = 0,
	.pApplicationInfo = &application_info,
	.enabledLayerCount = std::uint32_t(validation_enabled ? validation_layers.size() : 0),
	.ppEnabledLayerNames = validation_enabled ? validation_layers.data() : nullptr,
	.enabledExtensionCount = (std::uint32_t)all_extensions.size(),
	.ppEnabledExtensionNames = all_extensions.data()
    };

    Handle instance_;
    VULKAN_ASSERT(vkCreateInstance(&create_info, nullptr, &instance_)); 
    Unique_handle::operator=({ instance_, [](Handle _instance) { vkDestroyInstance(_instance, nullptr); } });

    if(validation_enabled) debug_messenger = {*this, validation_event_dispatcher} ;
}

Graphics::Instance::~Instance() {
    // Instance must be alive during the destruction of debug_messenger
    if(validation_enabled) debug_messenger = {};
    Unique_handle::operator=({});
    // validation_event_dispatcher must be destroyed after VkInstance, because destroying VkInstance may emit debug messages
}


