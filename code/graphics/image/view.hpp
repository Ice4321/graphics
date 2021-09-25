#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Image_view: public Utility::Unique_handle<VkImageView> {
    public:
	// TODO: Can format be stored in Image instead of passing it here?
	Image_view(class Logical_device* _logical_device, class Image& _image, VkFormat _format);
    
    private:
	class Logical_device* logical_device;
    };
}
