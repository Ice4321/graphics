#ifndef INCLUDED_GRAPHICS_VULKAN_INSTANCE_HPP
#define INCLUDED_GRAPHICS_VULKAN_INSTANCE_HPP

#include<vulkan/vulkan.h>

// TODO: rename this and files to instance instead of vulkan_instance
namespace Graphics {
    class Vulkan_instance {
    public:
	Vulkan_instance();

	operator VkInstance& () noexcept;

	~Vulkan_instance();

	Vulkan_instance(Vulkan_instance const&) = delete;
	Vulkan_instance& operator=(Vulkan_instance const&) = delete;

    private:
	VkInstance instance;

    };
}

#endif
