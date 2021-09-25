#pragma once

#include "utility/unique_handle.hpp"
#include <vulkan/vulkan.h>
#include "graphics/commands/queue.hpp"

// TODO: Rename this directory from 'devices' to 'device'

namespace Graphics {
    class Logical_device: public Utility::Unique_handle<VkDevice> {
    public:
	Logical_device(class Physical_device& _physical_device, class Surface& _surface);
	
	Queue const& get_graphics_queue() const noexcept;
	Queue const& get_presentation_queue() const noexcept;

	void submit_drawing_commands(class Command_buffer& _command_buffer, class Semaphore& _wait_sem, class Semaphore& _signal_sem);
	void present(class Swap_chain& _swap_chain, std::uint32_t _swap_chain_image_index, class Semaphore& _wait_sem);

	void wait_idle();

    private:
	// These might be the same queue
	Queue graphics_queue;
	Queue presentation_queue;

    };
}
