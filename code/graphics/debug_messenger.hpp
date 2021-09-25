#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Debug_messenger final: public Utility::Unique_handle<VkDebugUtilsMessengerEXT> {
    public:
	// Nothing accesses instance if Unique_handle is default-constructed, so the value of instance doesn't matter
	Debug_messenger() = default;
	Debug_messenger(class Instance* _instance, class Validation_event_dispatcher& _validation_event_dispatcher);
	
    private:
	class Instance* instance;

    };
}
