#pragma once

#include <shaderc/shaderc.h>
#include <string_view>
#include "graphics/shader_binary.hpp"
#include "graphics/shader/kind.hpp"
#include "utility/unique_handle.hpp"

namespace Graphics {
    class Shader_compiler {
    public:
	Shader_compiler();
	
	Shader_binary compile(Shader_kind const& _kind, std::string_view _source);

    private:
	Utility::Unique_handle<shaderc_compiler_t> compiler;
	Utility::Unique_handle<shaderc_compile_options_t> compile_options;

    };
}

