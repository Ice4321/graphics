#pragma once

#include <GLFW/glfw3.h>
#include <span>
#include "events/emitter.hpp"
#include "utility/unique_handle.hpp"


#include<tuple>

namespace Events::Graphics::Window {
    struct Close { };
    struct Resize { int framebuffer_width_px, framebuffer_height_px; };
}

namespace Graphics {
    class Window:
	public Utility::Unique_handle<GLFWwindow*>,
	public Event_emitter<
	    Events::Graphics::Window::Close,
	    Events::Graphics::Window::Resize
	>
    {
    public:
	static std::string get_glfw_version_string();
	static std::span<char const* const> get_required_instance_extensions();

	// Must be called on the main thread
	// Size given in screen coordinates (not pixels)
	Window(int _width_sc, int _height_sc);

	int get_framebuffer_width_px() const noexcept;
	int get_framebuffer_height_px() const noexcept;
	
	static void poll_events();
	static void await_events();

    private:
	// Number of all windows created
	inline static unsigned total_window_count = 0;
	inline static bool glfw_initialised = false;

	static void initialise_glfw();
	static void terminate_glfw();
	
	int framebuffer_width_px, framebuffer_height_px;

    };
}

