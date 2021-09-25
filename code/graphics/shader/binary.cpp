#include "graphics/shader/binary.hpp"

Graphics::Shader_binary::Shader_binary(Utility::Unique_handle<shaderc_compilation_result_t>&& _compilation_result):
    compilation_result(std::move(_compilation_result)),
    data(reinterpret_cast<std::byte const*>(shaderc_result_get_bytes(compilation_result)), shaderc_result_get_length(compilation_result))
{ }

std::span<std::byte const> const& Graphics::Shader_binary::get_data() const noexcept {
    return data;
}
