#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Framebuffer: public Utility::Unique_handle<VkFramebuffer> {
    public:
	Framebuffer(class Logical_device& _logical_device, class Render_pass& _render_pass, class Image_view& _image_view);

    };
}
