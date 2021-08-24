#ifndef INCLUDED_GRAPHICS_WINDOW_HPP
#define INCLUDED_GRAPHICS_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>

namespace Events {
    namespace Graphics {
	namespace Window {
	    struct Resize {
		int width, height;
	    };
	}
    }
}

namespace Graphics {
    namespace Events = Events::Graphics::Window;

    class Window {
    public:
	Window(int _width, int _height);

	int get_width() const noexcept;
	int get_height() const noexcept;

	~Window();
	
	Window(Window const&) = delete;
	Window& operator=(Window const&) = delete;

    private:
	int width, height;
	GLFWwindow* window;

    };
}

#endif
