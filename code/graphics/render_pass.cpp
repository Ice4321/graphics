#include "graphics/render_pass.hpp"
#include "graphics/utility/vulkan_assert.hpp"
#include "graphics/device/logical.hpp"

Graphics::Render_pass::Render_pass(
	Logical_device& _logical_device, 
	std::span<VkAttachmentDescription const> _attachments, 
	std::span<VkSubpassDescription const> _subpasses,
	std::span<VkSubpassDependency const> _subpass_dependencies
) {
    VkRenderPassCreateInfo create_info{
	.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
	.pNext = nullptr,
	.flags = 0,
	.attachmentCount = (std::uint32_t)_attachments.size(),
	.pAttachments = _attachments.data(),
	.subpassCount = (std::uint32_t)_subpasses.size(),
	.pSubpasses = _subpasses.data(),
	.dependencyCount = (std::uint32_t)_subpass_dependencies.size(),
	.pDependencies = _subpass_dependencies.data()
    };

    Handle render_pass;
    VULKAN_ASSERT(vkCreateRenderPass(_logical_device, &create_info, nullptr, &render_pass));
    Unique_handle::operator=({render_pass, [&_logical_device](Handle _render_pass) { vkDestroyRenderPass(_logical_device, _render_pass, nullptr); }});
}
