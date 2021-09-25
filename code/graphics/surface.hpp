#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>

namespace Graphics {
    class Surface: public Utility::Unique_handle<VkSurfaceKHR> {
    public:
	Surface(class Instance* _instance, class Window& _window);

	bool test_queue_family_presentation_support(class Physical_device& _physical_device, std::uint32_t _queue_family_index);

    private:
	class Instance* instance;

    };
}

