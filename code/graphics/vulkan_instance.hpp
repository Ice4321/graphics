#ifndef INCLUDED_GRAPHICS_VULKAN_INSTANCE_HPP
#define INCLUDED_GRAPHICS_VULKAN_INSTANCE_HPP

#include<vulkan/vulkan.h>

// TODO: rename this and files to instance instead of vulkan_instance
namespace Graphics {
    class Vulkan_instance {
    public:
	struct Use_validation_layers { bool value; };

	Vulkan_instance(Use_validation_layers _use_validation_layers);

	operator VkInstance& () noexcept;

	~Vulkan_instance();

	Vulkan_instance(Vulkan_instance const&) = delete;
	Vulkan_instance& operator=(Vulkan_instance const&) = delete;

    private:
	VkInstance instance;

    };
}

#endif
