#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Image: public Utility::Unique_handle<VkImage> {
    public:
	// Ctor tag indicating that this image should not be destroyed
	inline static constexpr struct Borrowed {} borrowed = {};

	Image(VkImage _image, Borrowed);

    };
}
