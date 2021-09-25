#ifndef INCLUDED_GRAPHICS_SHADER_BINARY_HPP
#define INCLUDED_GRAPHICS_SHADER_BINARY_HPP

#include<shaderc/shaderc.h>
#include<span>
#include "shader/kind.hpp"

// TODO: add move ctor/assignment and default ctor (compilation may throw so local variables in the try block get destroyed)
namespace Graphics {
    // SPIR-V binary
    class Shader_binary {
    public:
	// Takes ownership of _compilation_result
	Shader_binary(Shader_kind const& _shader_kind, shaderc_compilation_result_t _compilation_result);
	
	// libshaderc guarantees that the binary is castable to a uint32_t* 
	std::span<std::byte const> const& get_spirv_binary() const noexcept;

	~Shader_binary();

	Shader_binary(Shader_binary const&) = delete;
	Shader_binary& operator=(Shader_binary const&) = delete;

    private:
	Shader_kind shader_kind; // TODO: this field is not needed
	shaderc_compilation_result_t compilation_result;
	std::span<std::byte const> spirv_binary;
    };
}

#endif
