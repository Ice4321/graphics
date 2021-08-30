#include<iostream>
#include"concurrency/main_thread.hpp"
#include"graphics/window.hpp"
#include"graphics/instance.hpp"
#include"graphics/physical_device.hpp"
#include"graphics/surface.hpp"
#include"graphics/logical_device.hpp"
#include"utility/critical_error.hpp"

int main() {
    Concurrency::main_thread_id = std::this_thread::get_id();
    
    // glfwGetVersionString() may be called before glfwInit()
    std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;

    Graphics::Window window(400, 400);
    Graphics::Instance instance(Graphics::Instance::Validation::enabled);
    Graphics::Surface surface(instance, window);

    auto all_physical_devices = Graphics::Physical_device::enumerate_all(instance);
    for(auto const& physical_device : all_physical_devices) {
	std::cout << "Device: " << physical_device.get_properties().deviceName << std::endl;
    }
    
    Graphics::Logical_device logical_device(all_physical_devices[0], surface);




    bool exit = false;

    window.add_event_callback([&](Events::Graphics::Window::Close const& _event) {
	(void)_event;
	std::cout << "Window closed" << std::endl;
	exit = true;
    });

    window.add_event_callback([&](Events::Graphics::Window::Resize const& _event) {
	std::cout << "Window resized: " << _event.width_px << "x" << _event.height_px << std::endl;
    });

    while(!exit) {
	window.await_events();
    }

}



