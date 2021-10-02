#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>
#include <span>

namespace Graphics {
    class Render_pass: public Utility::Unique_handle<VkRenderPass> {
    public:
	Render_pass() = default;

	Render_pass(
	    class Logical_device& _logical_device, 
	    std::span<VkAttachmentDescription const> _attachments, 
	    std::span<VkSubpassDescription const> _subpasses,
	    std::span<VkSubpassDependency const> _subpass_dependencies
	);

    };
}
