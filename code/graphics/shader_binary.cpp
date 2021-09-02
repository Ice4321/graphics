#include"graphics/shader_binary.hpp"

// TODO: safe to cast char* to std::byte*?
Graphics::Shader_binary::Shader_binary(Shader_kind _shader_kind, shaderc_compilation_result_t _compilation_result):
    shader_kind(_shader_kind),
    compilation_result(_compilation_result),
    spirv_binary(reinterpret_cast<std::byte const*>(shaderc_result_get_bytes(compilation_result)), shaderc_result_get_length(compilation_result))
{ }

Graphics::Shader_binary::~Shader_binary() {
    shaderc_result_release(compilation_result);
}

std::span<std::byte const> const& Graphics::Shader_binary::get_spirv_binary() const noexcept {
    return spirv_binary;
}
