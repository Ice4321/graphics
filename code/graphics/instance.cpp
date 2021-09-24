#include"graphics/instance.hpp"
#include"graphics/window.hpp"
#include<array>
#include "graphics/utility/vulkan_assert.hpp"
#include<iostream>

Graphics::Instance::Instance(Validation _validation):
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
    auto [glfw_extensions, glfw_extension_count] = Window::get_required_instance_extensions();
    for(std::size_t i = 0; i < glfw_extension_count; ++i) all_extensions.emplace_back(glfw_extensions[i]);
    if(validation_enabled) {
	all_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }


    debug_messenger.add_event_callback([](Events::Graphics::Debug_messenger::Message const& _message){
	std::cout << _message.text << std::endl;
    });

    // To capture events that occur while creating or destroying an instance an application can link a VkDebugUtilsMessengerCreateInfoEXT structure 
    // to the pNext element of the VkInstanceCreateInfo structure given to vkCreateInstance.
    // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/chap50.html#_examples_10
    VkDebugUtilsMessengerCreateInfoEXT const& debug_messenger_create_info = debug_messenger.get_creation_info();

    VkInstanceCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	.pNext = validation_enabled ? &debug_messenger_create_info : nullptr,
	.flags = 0,
	.pApplicationInfo = &application_info,
	.enabledLayerCount = std::uint32_t(validation_enabled ? validation_layers.size() : 0),
	.ppEnabledLayerNames = validation_enabled ? validation_layers.data() : nullptr,
	.enabledExtensionCount = (std::uint32_t)all_extensions.size(),
	.ppEnabledExtensionNames = all_extensions.data()
    };

    VkInstance instance;
    VULKAN_ASSERT(vkCreateInstance(&create_info, nullptr, &instance)); 
    Unique_handle::operator=({ instance, [](Handle _instance) { vkDestroyInstance(_instance, nullptr); } });

    if(validation_enabled) debug_messenger.initialise(this);

}

Graphics::Instance::~Instance() {
    if(validation_enabled) debug_messenger.destroy();
    Unique_handle::operator=({});
}


