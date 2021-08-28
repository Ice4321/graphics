#include"graphics/window.hpp"
#include"concurrency/main_thread.hpp"
#include"utility/critical_error.hpp"
#include<cassert>
#include<iostream>

Graphics::Window::Window(int _width, int _height):
    width(_width), height(_height), 
    window(nullptr)
{ 
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
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(width, height, "main", nullptr, nullptr);
    glfwSetWindowUserPointer(window, static_cast<void*>(this));

    glfwSetWindowCloseCallback(window, [](GLFWwindow* _window) {
	Window* window_object = static_cast<Window*>(glfwGetWindowUserPointer(_window));
	window_object->post_event(Events::Graphics::Window::Close{});
    });
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* _window, int _width, int _height) {
	Window* window_object = static_cast<Window*>(glfwGetWindowUserPointer(_window));
	window_object->width = _width;
	window_object->height = _height;
	window_object->post_event(Events::Graphics::Window::Resize{_width, _height});
    });

    ++total_window_count;
}

Graphics::Window::~Window() {
    glfwDestroyWindow(window);

    --total_window_count;
    
    if(glfw_initialised && total_window_count == 0) {
	glfwTerminate();
	glfw_initialised = false;
    }
}


int Graphics::Window::get_width() const noexcept {
    return width;
}

int Graphics::Window::get_height() const noexcept {
    return height;
}

void Graphics::Window::poll_events() {
    glfwPollEvents();
}

void Graphics::Window::await_events() {
    glfwWaitEvents();
}
