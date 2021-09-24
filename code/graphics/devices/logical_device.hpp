#pragma once

#include<vulkan/vulkan.h>
#include"graphics/devices/physical_device.hpp"
#include"graphics/surface.hpp"
#include"utility/unique_handle.hpp"
#include"graphics/commands/queue.hpp"
#include<vector>

namespace Graphics {
    class Logical_device: public Utility::Unique_handle<VkDevice> {
    public:
	Logical_device(Physical_device& _physical_device, Surface& _surface);
	
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
