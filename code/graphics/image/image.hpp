#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>
#include <vector>

namespace Graphics {
    class Image: public Utility::Unique_handle<VkImage> {
    public:
	static std::vector<Image> get_swap_chain_images(class Logical_device& _logical_device, class Swap_chain& _swap_chain);

	VkFormat get_format() const noexcept;
	VkExtent2D get_extent() const noexcept;

    private:
	Image(VkImage _image, class Swap_chain& _swap_chain);

	VkFormat format;
	VkExtent2D extent;
    };
}
