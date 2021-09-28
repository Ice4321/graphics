#include "graphics/shader/binary.hpp"

Graphics::Shader_binary::Shader_binary(Shader_kind const& _kind, Utility::Unique_handle<shaderc_compilation_result_t>&& _compilation_result):
    kind(_kind),
    compilation_result(std::move(_compilation_result)),
    data(reinterpret_cast<std::byte const*>(shaderc_result_get_bytes(compilation_result)), shaderc_result_get_length(compilation_result))
{ }

Graphics::Shader_kind const& Graphics::Shader_binary::get_kind() const noexcept {
    return kind;
}

std::span<std::byte const> const& Graphics::Shader_binary::get_data() const noexcept {
    return data;
}


