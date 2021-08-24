#include"graphics/window.hpp"
#include"preprocessor/macros.hpp"
#include"concurrency/main_thread.hpp"
#include<cassert>

Graphics::Window::Window(int _width, int _height):
    width(_width), height(_height),
    //If unique_ptr holds a nullptr, the deleter is not called
    //The deleter won't be called before GLFW is initialised
    window(nullptr, [](GLFWwindow* _window){ glfwDestroyWindow(_window); })
{ 
    IF_DEBUG(assert(std::this_thread::get_id() == Concurrency::main_thread_id));

    //Safe to call multiple times
    if(glfwInit() == GLFW_FALSE) throw std::runtime_error("Graphics::Window::Window(): GLFW initialisation error");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window.reset(glfwCreateWindow(width, height, "main", nullptr, nullptr));
    if(!window) throw std::runtime_error("Graphics::Window::Window(): window creation error");
}

Graphics::Window::~Window() {
    IF_DEBUG(assert(std::this_thread::get_id() == Concurrency::main_thread_id));

    glfwTerminate(); //Safe to call multiple times
}


int Graphics::Window::get_width() const noexcept {
    return width;
}

int Graphics::Window::get_height() const noexcept {
    return height;
}
