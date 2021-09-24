#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Semaphore: public Utility::Unique_handle<VkSemaphore> {
    public:
	// Address of _logical_device must not change
	Semaphore(class Logical_device* _logical_device);

    };
}
