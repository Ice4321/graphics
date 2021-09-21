#ifndef INCLUDED_GRAPHICS_INSTANCE_HPP
#define INCLUDED_GRAPHICS_INSTANCE_HPP

#include<vulkan/vulkan.h>
#include"graphics/validation_callback.hpp"
#include "utility/assert.hpp"

namespace Graphics {
    class Instance {
    public:
	enum struct Validation { enabled, disabled };

	Instance(Validation _validation);

	operator VkInstance& () noexcept;
	
	// TODO: Constrain this to function pointer types only
	template<typename _Function_ptr>
	_Function_ptr get_function_address(char const* _function_name);

	~Instance();
	
	// Copying and moving must be disallowed because Vulkan holds a reference to validation_callback
	// and Surface holds a reference to Instance
	Instance(Instance const&) = delete;
	Instance& operator=(Instance const&) = delete;

    private:
	bool validation_enabled;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debug_messenger;
	Validation_callback validation_callback;

    };

    template<typename _Function_ptr>
    _Function_ptr Instance::get_function_address(char const* _function_name) {
	_Function_ptr pointer = reinterpret_cast<_Function_ptr>(vkGetInstanceProcAddr(instance, _function_name));
	ASSERT(pointer);
	return pointer;
    }
}

#endif
