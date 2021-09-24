#ifndef INCLUDED_GRAPHICS_INSTANCE_HPP
#define INCLUDED_GRAPHICS_INSTANCE_HPP

#include<vulkan/vulkan.h>
#include"graphics/validation_callback.hpp"
#include "utility/assert.hpp"
#include "graphics/debug_messenger.hpp"

namespace Graphics {
    class Instance final: public Utility::Unique_handle<VkInstance> {
    public:
	enum struct Validation { enabled, disabled };

	Instance(Validation _validation);

	// TODO: Constrain this to function pointer types only
	template<typename _Function_ptr>
	_Function_ptr get_function_address(char const* _function_name);

	~Instance();

    private:
	bool validation_enabled;
	Debug_messenger debug_messenger;

    };

    template<typename _Function_ptr>
    _Function_ptr Instance::get_function_address(char const* _function_name) {
	_Function_ptr pointer = reinterpret_cast<_Function_ptr>(vkGetInstanceProcAddr(*this, _function_name));
	ASSERT(pointer);
	return pointer;
    }
}

#endif
