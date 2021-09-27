#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Debug_messenger: public Utility::Unique_handle<VkDebugUtilsMessengerEXT> {
    public:
	Debug_messenger() = default;
	Debug_messenger(class Instance& _instance, class Validation_event_dispatcher& _validation_event_dispatcher);
	
    };
}
