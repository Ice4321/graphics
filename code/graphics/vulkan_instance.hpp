#ifndef INCLUDED_GRAPHICS_VULKAN_INSTANCE_HPP
#define INCLUDED_GRAPHICS_VULKAN_INSTANCE_HPP

#include<vulkan/vulkan.h>
#include<memory>
#include"utility/critical_error.hpp"
#include"graphics/validation_callback.hpp"

// TODO: rename this and files to instance instead of vulkan_instance
namespace Graphics {
    class Vulkan_instance {
    public:
	enum struct Validation { enabled, disabled };

	Vulkan_instance(Validation _validation);

	operator VkInstance& () noexcept;
	
	// TODO: Constrain this to function pointer types only
	template<typename _Function_ptr>
	_Function_ptr get_function_address(char const* _function_name);

	~Vulkan_instance();

	Vulkan_instance(Vulkan_instance const&) = delete;
	Vulkan_instance& operator=(Vulkan_instance const&) = delete;

    private:
	bool validation_enabled;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debug_messenger;
	// If address of Vulkan_instance changes (such as due to copying or moving), the address of validation_callback must not change,
	// because it is registered by Vulkan
	std::unique_ptr<Validation_callback> validation_callback;
	

    };

    template<typename _Function_ptr>
    _Function_ptr Vulkan_instance::get_function_address(char const* _function_name) {
	_Function_ptr pointer = reinterpret_cast<_Function_ptr>(vkGetInstanceProcAddr(instance, _function_name));
	if(!pointer) critical_error("vkGetInstanceProcAddr()");
	return pointer;
    }
}

#endif
