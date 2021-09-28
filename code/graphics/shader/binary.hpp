#pragma once

#include <shaderc/shaderc.h>
#include <span>
#include "graphics/shader/kind.hpp"
#include "utility/unique_handle.hpp"

namespace Graphics {
    // SPIR-V binary
    class Shader_binary {
    public:
	// Takes ownership of _compilation_result
	Shader_binary(Shader_kind const& _kind, Utility::Unique_handle<shaderc_compilation_result_t>&& _compilation_result);
	
	Shader_kind const& get_kind() const noexcept;

	// libshaderc guarantees that the binary is castable to a uint32_t* 
	std::span<std::byte const> const& get_data() const noexcept;

    private:
	Shader_kind kind;
	Utility::Unique_handle<shaderc_compilation_result_t> compilation_result;
	std::span<std::byte const> data;
    };
}

