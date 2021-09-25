#include "graphics/wsi/window.hpp"
#include "utility/critical_error.hpp"

std::string Graphics::Window::get_glfw_version_string() {
    // May be called before glfwInit()
    return {glfwGetVersionString()};
}

std::span<char const* const> Graphics::Window::get_required_instance_extensions() {
    initialise_glfw();
    static std::span<char const* const> const result = []() -> std::span<char const* const> {
	std::uint32_t count = 0;
	char const* const* extensions = glfwGetRequiredInstanceExtensions(&count);
	return {extensions, count};
    }();

    return result;
}

Graphics::Window::Window(int _width_sc, int _height_sc) { 
    initialise_glfw();   
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Error callback is invoked in case of failure
    Unique_handle::operator=({
	glfwCreateWindow(_width_sc, _height_sc, "main", nullptr, nullptr),
	[](Handle _window) { 
	    glfwDestroyWindow(_window);
	    --total_window_count;
	    terminate_glfw();
	}
    });

    ++total_window_count;

    glfwGetFramebufferSize(*this, &framebuffer_width_px, &framebuffer_height_px);
    glfwSetWindowUserPointer(*this, static_cast<void*>(this));
    glfwSetWindowCloseCallback(*this, [](GLFWwindow* _window) {
	Window* window_object = static_cast<Window*>(glfwGetWindowUserPointer(_window));
	window_object->post_event(Events::Graphics::Window::Close{});
    });
    glfwSetFramebufferSizeCallback(*this, [](GLFWwindow* _window, int _framebuffer_width_px, int _framebuffer_height_px) {
	Window* window_object = static_cast<Window*>(glfwGetWindowUserPointer(_window));
	window_object->framebuffer_width_px = _framebuffer_width_px;
	window_object->framebuffer_height_px = _framebuffer_height_px;
	window_object->post_event(Events::Graphics::Window::Resize{_framebuffer_width_px, _framebuffer_height_px});
    });
}

int Graphics::Window::get_framebuffer_width_px() const noexcept {
    return framebuffer_width_px;
}

int Graphics::Window::get_framebuffer_height_px() const noexcept {
    return framebuffer_height_px;
}

void Graphics::Window::poll_events() {
    glfwPollEvents();
}

void Graphics::Window::await_events() {
    glfwWaitEvents();
}

void Graphics::Window::initialise_glfw() {
    if(!glfw_initialised) {
	// May be called before glfwInit()
	glfwSetErrorCallback([](int _error_code, char const* _description){ 
	    (void)_error_code;
	    // TODO: use std::format
	    critical_error(std::string("GLFW error: ") + std::string(_description));
	});

	glfwInit();
	glfw_initialised = true;
    }
}

void Graphics::Window::terminate_glfw() {
    if(glfw_initialised && total_window_count == 0) {
	glfwTerminate();
	glfw_initialised = false;
    }
}

