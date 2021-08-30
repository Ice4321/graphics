#include"graphics/window.hpp"
#include"concurrency/main_thread.hpp"
#include"utility/critical_error.hpp"
#include<cassert>
#include<iostream>

std::tuple<char const* const*, uint32_t> Graphics::Window::get_required_instance_extensions() {
    initialise_glfw();
    uint32_t count = 0;
    char const* const* extensions = glfwGetRequiredInstanceExtensions(&count);

    return { extensions, count };
}

Graphics::Window::Window(int _width_sc, int _height_sc):
    window(nullptr)
{ 
    initialise_glfw();   

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(_width_sc, _height_sc, "main", nullptr, nullptr);

    glfwGetFramebufferSize(window, &width_px, &height_px);

    glfwSetWindowUserPointer(window, static_cast<void*>(this));

    glfwSetWindowCloseCallback(window, [](GLFWwindow* _window) {
	Window* window_object = static_cast<Window*>(glfwGetWindowUserPointer(_window));
	window_object->post_event(Events::Graphics::Window::Close{});
    });

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* _window, int _width_px, int _height_px) {
	Window* window_object = static_cast<Window*>(glfwGetWindowUserPointer(_window));
	window_object->width_px = _width_px;
	window_object->height_px = _height_px;
	window_object->post_event(Events::Graphics::Window::Resize{_width_px, _height_px});
    });

    ++total_window_count;
}

Graphics::Window::~Window() {
    glfwDestroyWindow(window);

    --total_window_count;
    
    terminate_glfw();
}

Graphics::Window::operator GLFWwindow*& () noexcept {
    return window;
}

int Graphics::Window::get_width_px() const noexcept {
    return width_px;
}

int Graphics::Window::get_height_px() const noexcept {
    return height_px;
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

