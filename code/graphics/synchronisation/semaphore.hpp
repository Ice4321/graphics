#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Semaphore final: public Utility::Unique_handle<VkSemaphore> {
    public:
	Semaphore(class Logical_device* _logical_device);

    private:
	class Logical_device* logical_device;

    };
}
