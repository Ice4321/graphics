#include "graphics/rendering/framebuffer.hpp"
#include "graphics/utility/vulkan_assert.hpp"
#include "graphics/device/logical.hpp"
#include "graphics/rendering/render_pass.hpp"
#include "graphics/image/view.hpp"

Graphics::Framebuffer::Framebuffer(Logical_device& _logical_device, Render_pass& _render_pass, Image_view& _image_view) {
    VkImageView attachments[] = {_image_view};

    VkFramebufferCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.renderPass = _render_pass,
	.attachmentCount = 1,
	.pAttachments = attachments,
	.width = _image_view.get_image_extent().width,
	.height = _image_view.get_image_extent().height,
	.layers = 1
    };

    Handle framebuffer;
    VULKAN_ASSERT(vkCreateFramebuffer(_logical_device, &create_info, nullptr, &framebuffer));
    Unique_handle::operator=({framebuffer, [&_logical_device](Handle _framebuffer) { vkDestroyFramebuffer(_logical_device, _framebuffer, nullptr); }});
}


