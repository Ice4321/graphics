#ifndef INCLUDED_GRAPHICS_SHADER_COMPILER_HPP
#define INCLUDED_GRAPHICS_SHADER_COMPILER_HPP

#include<shaderc/shaderc.h>
#include<stdexcept>
#include<memory>
#include<string_view>
#include"graphics/shader_binary.hpp"

namespace Exceptions {
    namespace Graphics {
	namespace Shader_compiler {
	    struct Compilation_error: public std::runtime_error { using std::runtime_error::runtime_error; };
	}
    }
}

namespace Graphics {
    class Shader_compiler {
    public:
	enum struct Shader_kind { vertex, fragment, compute, geometry, tessellation_control, tessellation_evaluation };

	Shader_compiler();
	
	Shader_binary compile(Shader_kind _kind, std::string_view _source);

	~Shader_compiler();

	Shader_compiler(Shader_compiler const&) = delete;
	Shader_compiler& operator=(Shader_compiler const&) = delete;

    private:
	shaderc_compiler_t compiler;
	shaderc_compile_options_t compile_options;

    };
}

#endif
