#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Image_view: public Utility::Unique_handle<VkImageView> {
    public:
	Image_view(class Image& _image);

    };
}
