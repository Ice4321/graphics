#include "graphics/image/view.hpp"
#include "graphics/image/image.hpp"
#include "graphics/state/globals.hpp"
#include "graphics/utility/vulkan_assert.hpp"

Graphics::Image_view::Image_view(Image& _image) {
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
    VULKAN_ASSERT(vkCreateImageView(*logical_device, &create_info, nullptr, &image_view)); 
    Unique_handle::operator=({image_view, [](Handle _image_view) { vkDestroyImageView(*logical_device, _image_view, nullptr); }});
}
