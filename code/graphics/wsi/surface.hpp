#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Surface: public Utility::Unique_handle<VkSurfaceKHR> {
    public:
	Surface(class Window& _window);

    };
}

