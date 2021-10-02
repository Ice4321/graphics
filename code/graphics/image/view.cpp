#include "graphics/image/view.hpp"
#include "graphics/image/image.hpp"
#include "graphics/device/logical.hpp"
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Image_view::Image_view(Logical_device& _logical_device, Image& _image):
    image_format(_image.get_format()),
    image_extent(_image.get_extent())
{
    VkImageViewCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.image = _image,
	.viewType = VK_IMAGE_VIEW_TYPE_2D,
	.format = _image.get_format(),
	.components = {
	    VK_COMPONENT_SWIZZLE_IDENTITY,
	    VK_COMPONENT_SWIZZLE_IDENTITY,
	    VK_COMPONENT_SWIZZLE_IDENTITY,
	    VK_COMPONENT_SWIZZLE_IDENTITY
	},
	.subresourceRange = {
	    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
	    .baseMipLevel = 0,
	    .levelCount = 1,
	    .baseArrayLayer = 0,
	    .layerCount = 1
	}
    };

    Handle image_view;
    VULKAN_ASSERT(vkCreateImageView(_logical_device, &create_info, nullptr, &image_view)); 
    Unique_handle::operator=({image_view, [&_logical_device](Handle _image_view) { vkDestroyImageView(_logical_device, _image_view, nullptr); }});
}

VkFormat Graphics::Image_view::get_image_format() const noexcept { 
    return image_format;
}

VkExtent2D Graphics::Image_view::get_image_extent() const noexcept {
    return image_extent;
}

