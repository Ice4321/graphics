cmake_minimum_required(VERSION 3.10)
project(graphics)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
add_compile_options(-Wall -Wextra -Wpedantic -O3 -std=c++20)
add_link_options(-Wall -Wextra -Wpedantic -O3 -std=c++20)

file(GLOB_RECURSE Sources CONFIGURE_DEPENDS "code/*.cpp")
add_executable(main ${Sources})

target_include_directories(main PUBLIC code)


