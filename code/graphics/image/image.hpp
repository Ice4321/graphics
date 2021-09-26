#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Image: public Utility::Unique_handle<VkImage> {
    public:
	// Ctor tag indicating that this image should not be destroyed
	inline static constexpr struct Borrowed {} borrowed = {};

	Image(Borrowed, VkImage _image, VkFormat _format, VkExtent2D _extent);
	
	VkFormat const& get_format() const noexcept;
	VkExtent2D const& get_extent() const noexcept;

    private:
	VkFormat format;
	VkExtent2D extent;
    };
}
