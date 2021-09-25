#ifndef INCLUDED_GRAPHICS_INSTANCE_HPP
#define INCLUDED_GRAPHICS_INSTANCE_HPP

#include <vulkan/vulkan.h>
#include <functional>
#include "utility/assert.hpp"
#include "graphics/debug_messenger.hpp"
#include "graphics/validation_event_dispatcher.hpp"

namespace Graphics {
    class Instance final: public Utility::Unique_handle<VkInstance> {
    public:
	enum struct Validation { enabled, disabled };
	
	// Validation callback must be registered in the constructor in order to catch messages emitted during the construction of VkInstance
	Instance(
	    Validation _validation, 
	    std::function<void(Events::Graphics::Validation_event_dispatcher::Message const&)>&& _validation_message_callback = 
		+[](Events::Graphics::Validation_event_dispatcher::Message const&){}
	);

	// TODO: Constrain this to function pointer types only
	template<typename _Function_ptr> _Function_ptr get_function_address(char const* _function_name);

	~Instance();

    private:
	bool validation_enabled;
	Validation_event_dispatcher validation_event_dispatcher;
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
