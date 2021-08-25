#ifndef INCLUDED_GRAPHICS_WINDOW_HPP
#define INCLUDED_GRAPHICS_WINDOW_HPP

#include<GLFW/glfw3.h>
#include"events/emitter.hpp"
#include"preprocessor/macros.hpp"

namespace Events {
    namespace Graphics {
	namespace Window {
	    struct Close { };
	    struct Resize { int width, height; };
	}
    }
}

namespace Graphics {
    class Window: 
	public Event_emitter<
	    Events::Graphics::Window::Close,
	    Events::Graphics::Window::Resize
	>
    {
    public:
	// Must be called on the main thread
	Window(int _width, int _height);

	// == Inherited from Event_emitter ==
	// Callback_handle add_event_callback(Callback _callback);
	// void remove_event_callback(Callback_handle _handle);

	int get_width() const noexcept;
	int get_height() const noexcept;
	
	static void poll_events();
	static void await_events();

	~Window();
	
	Window(Window const&) = delete;
	Window& operator=(Window const&) = delete;

    private:
	// Number of all windows created
	inline static unsigned total_window_count = 0;
	inline static bool glfw_initialised = false;

	int width, height;
	GLFWwindow* window;

    };
}

#endif
