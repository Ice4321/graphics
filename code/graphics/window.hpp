#ifndef INCLUDED_GRAPHICS_WINDOW_HPP
#define INCLUDED_GRAPHICS_WINDOW_HPP

#include<GLFW/glfw3.h>
#include<tuple>
#include"events/emitter.hpp"
#include"preprocessor/macros.hpp"

namespace Events {
    namespace Graphics {
	namespace Window {
	    struct Close { };
	    struct Resize { int width_px, height_px; };
	}
    }
}

// TODO: rename width_px / height_px to framebuffer_width_px / framebuffer_height_px (also in the event struct)

namespace Graphics {
    class Window: 
	public Event_emitter<
	    Events::Graphics::Window::Close,
	    Events::Graphics::Window::Resize
	>
    {
    public:
	static std::tuple<char const* const*, uint32_t> get_required_instance_extensions();

	// Must be called on the main thread
	// Size given in screen coordinates (not pixels)
	Window(int _width_sc, int _height_sc);
	
	operator GLFWwindow*& () noexcept;

	int get_width_px() const noexcept;
	int get_height_px() const noexcept;
	
	static void poll_events();
	static void await_events();

	~Window();
	
	Window(Window const&) = delete;
	Window& operator=(Window const&) = delete;

    private:
	// Number of all windows created
	inline static unsigned total_window_count = 0;
	inline static bool glfw_initialised = false;

	static void initialise_glfw();
	static void terminate_glfw();
	
	int width_px, height_px;
	GLFWwindow* window;

    };
}

#endif
