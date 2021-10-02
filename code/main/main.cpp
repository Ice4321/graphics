#include<iostream>
#include"concurrency/main_thread.hpp"
#include"graphics/wsi/window.hpp"
#include"graphics/state/instance.hpp"
#include"graphics/device/physical.hpp"
#include"graphics/device/logical.hpp"
#include"graphics/wsi/surface.hpp"
#include"graphics/wsi/swap_chain.hpp"
#include"graphics/shader/compiler.hpp"
#include"graphics/rendering/renderer.hpp"
#include"graphics/shader/module.hpp"
#include"utility/unique_handle.hpp"
#include"utility/assert.hpp"

int main() {
    Concurrency::main_thread_id = std::this_thread::get_id();
    
    std::cout << "GLFW version: " << Graphics::Window::get_glfw_version_string() << std::endl;

    Graphics::Window window(800, 600);
    
    Graphics::Instance instance = {
	Graphics::Instance::Validation::enabled, 
	[](Events::Graphics::Validation_event_dispatcher::Message const& _m) {
	    if(_m.severity == Events::Graphics::Validation_event_dispatcher::Message::Severity::error) critical_error(_m.message);
	    else if(_m.severity >= Events::Graphics::Validation_event_dispatcher::Message::Severity::info) std::cout << _m.message << std::endl;
	    //else if(_m.severity >= Events::Graphics::Validation_event_dispatcher::Message::Severity::verbose) std::cout << _m.message << std::endl;
	}
    };

    Graphics::Surface surface(instance, window);

    auto all_physical_devices = Graphics::Physical_device::enumerate_all(instance);
    for(auto const& physical_device : all_physical_devices) {
	std::cout << "Device: " << physical_device.get_properties().deviceName << std::endl;
    }
    
    Graphics::Logical_device logical_device = {all_physical_devices[0], surface};

    Graphics::Swap_chain swap_chain(logical_device, surface, window);

    Graphics::Shader_compiler shader_compiler;
    
    auto vertex_shader_binary = shader_compiler.compile(
	Graphics::Shader_kinds::vertex,
	R"(
	    #version 450

	    layout(location = 0) out vec3 fragment_colour;

	    vec2 positions[3] = vec2[](
		vec2(0.0, -0.5),
		vec2(0.5, 0.5),
		vec2(-0.5, 0.5)
	    );

	    vec3 colours[3] = vec3[](
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, 1.0, 0.0),
		vec3(0.0, 0.0, 1.0)
	    );

	    void main() {
		gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
		fragment_colour = colours[gl_VertexIndex];
	    }
	)"
    );

    auto fragment_shader_binary = shader_compiler.compile(
	Graphics::Shader_kinds::fragment,
	R"(
	    #version 450
    
	    layout(location = 0) in vec3 fragment_colour;
	    layout(location = 0) out vec4 out_colour;

	    void main() {
		out_colour = vec4(fragment_colour, 1.0);
	    }

	)"
    );

    Graphics::Shader_module vertex_shader_module(logical_device, vertex_shader_binary);
    Graphics::Shader_module fragment_shader_module(logical_device, fragment_shader_binary);

    Graphics::Renderer renderer(logical_device, swap_chain, vertex_shader_module, fragment_shader_module);

    renderer.draw_frame();

    bool exit = false;

    window.add_event_callback([&](Events::Graphics::Window::Close const& _event) {
	(void)_event;
	std::cout << "Window closed" << std::endl;
	exit = true;
    });

    window.add_event_callback([&](Events::Graphics::Window::Resize const& _event) {
	std::cout << "Window resized: " << _event.framebuffer_width_px << "x" << _event.framebuffer_height_px << std::endl;
    });

    while(!exit) {
	window.await_events();
    }

    logical_device.wait_idle();

}



