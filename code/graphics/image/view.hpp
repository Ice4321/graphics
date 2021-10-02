#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Image_view: public Utility::Unique_handle<VkImageView> {
    public:
	Image_view(class Logical_device& _logical_device, class Image& _image);

	VkFormat get_image_format() const noexcept;
	VkExtent2D get_image_extent() const noexcept;

    private:
	VkFormat image_format;
	VkExtent2D image_extent;
    };
}
