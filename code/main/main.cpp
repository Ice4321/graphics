#include<iostream>
#include"concurrency/main_thread.hpp"
#include"graphics/window.hpp"
#include"graphics/physical_device.hpp"

int main() {
    Concurrency::main_thread_id = std::this_thread::get_id();
    
    // TODO: std::format
    
    // glfwGetVersionString() may be called before glfwInit()
    std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;

    Graphics::Window w1(400, 400);
    bool exit = false;

    w1.add_event_callback([&](Events::Graphics::Window::Close const& _event) {
	(void)_event;
	std::cout << "Window closed" << std::endl;
	exit = true;
    });

    w1.add_event_callback([&](Events::Graphics::Window::Resize const& _event) {
	std::cout << "Window resized: " << _event.width << "x" << _event.height << std::endl;
    });

    while(!exit) {
	w1.await_events();
    }

}



