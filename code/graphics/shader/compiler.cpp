#include "graphics/shader/compiler.hpp"
#include "graphics/shader/compilation_status.hpp"
#include "utility/assert.hpp"

Graphics::Shader_compiler::Shader_compiler() {
    shaderc_compiler_t compiler_ = shaderc_compiler_initialize();
    shaderc_compile_options_t compile_options_ = shaderc_compile_options_initialize();

    ASSERT(compiler_);
    ASSERT(compile_options_);

    compiler = {compiler_, [](decltype(compiler)::Handle _compiler) { shaderc_compiler_release(_compiler); }};
    compile_options = {compile_options_, [](decltype(compile_options)::Handle _compile_options) { shaderc_compile_options_release(_compile_options); }};
    
    shaderc_compile_options_set_source_language(compile_options, shaderc_source_language_glsl);
    shaderc_compile_options_set_optimization_level(compile_options, shaderc_optimization_level_performance);
    shaderc_compile_options_set_target_env(compile_options, shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

    // TODO: shaderc_compile_options_add_macro_definition()
    // TODO: shaderc_compile_options_set_include_callbacks()
}

Graphics::Shader_binary Graphics::Shader_compiler::compile(Shader_kind const& _kind, std::string_view _source) {
    // The input_file_name argument used as a tag to identify the source string in cases like emitting error messages. 
    // It doesn't have to be a 'file name'.
    shaderc_compilation_result_t compilation_result = shaderc_compile_into_spv(
	compiler, _source.data(), _source.length(), _kind, _kind, "main", compile_options
    );
    
    // Some internal error, not a compilation error
    ASSERT(compilation_result);

    shaderc_compilation_status compilation_status = shaderc_result_get_compilation_status(compilation_result);
    std::size_t error_count = shaderc_result_get_num_errors(compilation_result);
    std::size_t warning_count = shaderc_result_get_num_warnings(compilation_result);

    if(compilation_status != shaderc_compilation_status_success || warning_count || error_count) {
	// TODO: std::format
	std::string message = "Shader compilation failed (" + std::string(shader_compilation_status_string(compilation_status)) + 
	    std::string(") with ") + std::to_string(warning_count) + std::string(" warning(s) and ") + 
	    std::to_string(error_count) + " error(s):\n" +
	    std::string(shaderc_result_get_error_message(compilation_result));

	shaderc_result_release(compilation_result);

	critical_error(message);
    } else {
	// Transfer ownership
	return {_kind, compilation_result};
    }
}




