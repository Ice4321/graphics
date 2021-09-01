#include"graphics/shader_compiler.hpp"
#include"utility/critical_error.hpp"

Graphics::Shader_compiler::Shader_compiler() {
    // Calling shaderc_compiler_initialize() on multiple threads needs no synchronisation
    if((compiler = shaderc_compiler_initialize()) == nullptr) critical_error("shaderc_compiler_initialize()");
    if((compile_options = shaderc_compile_options_initialize()) == nullptr) critical_error("shaderc_compile_options_initialize()");
    
    shaderc_compile_options_set_source_language(compile_options, shaderc_source_language_glsl);
    shaderc_compile_options_set_optimization_level(compile_options, shaderc_optimization_level_performance);
    shaderc_compile_options_set_target_env(compile_options, shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
    // Is there a way to retrieve the warnings otherwise?
    shaderc_compile_options_set_warnings_as_errors(compile_options);

    // TODO: shaderc_compile_options_add_macro_definition()
    // TODO: shaderc_compile_options_set_include_callbacks()
}

Graphics::Shader_compiler::~Shader_compiler() {
    shaderc_compile_options_release(compile_options);
    shaderc_compiler_release(compiler);
}

Graphics::Shader_binary Graphics::Shader_compiler::compile(Shader_kind _kind, std::string_view _source) {
    shaderc_shader_kind kind{};
    switch(_kind) {
	case Shader_kind::vertex: kind = shaderc_vertex_shader; break;
	case Shader_kind::fragment: kind = shaderc_fragment_shader; break;
	case Shader_kind::compute: kind = shaderc_compute_shader; break;
	case Shader_kind::geometry: kind = shaderc_geometry_shader; break;
	case Shader_kind::tessellation_control: kind = shaderc_tess_control_shader; break;
	case Shader_kind::tessellation_evaluation: kind = shaderc_tess_evaluation_shader; break;
    }
    
    // The input_file_name argument used as a tag to identify the source string in cases like emitting error messages. 
    // It doesn't have to be a 'file name'.
    
    char const* file_name = nullptr;
    switch(_kind) {
	case Shader_kind::vertex: file_name = "vertex"; break;
	case Shader_kind::fragment: file_name = "fragment"; break;
	case Shader_kind::compute: file_name = "compute"; break;
	case Shader_kind::geometry: file_name = "geometry"; break;
	case Shader_kind::tessellation_control: file_name = "tess control"; break;
	case Shader_kind::tessellation_evaluation: file_name = "tess eval"; break;
    }

    shaderc_compilation_result_t compilation_result = shaderc_compile_into_spv(
	compiler,
	_source.data(), _source.length(),
	kind, 
	file_name,
	"main",
	compile_options
    );
    
    // Some internal error, not a compilation error
    if(!compilation_result) critical_error("shaderc_compile_into_spv()");

    shaderc_compilation_status compilation_status = shaderc_result_get_compilation_status(compilation_result);
    
    if(compilation_status == shaderc_compilation_status_success) {
	// Transfer ownership
	return { compilation_result };
    } else {
	char const* compilation_status_message = nullptr;
	switch(compilation_status) {
	    case shaderc_compilation_status_success: compilation_status_message = "success"; break; // Silence warning
	    case shaderc_compilation_status_invalid_stage: compilation_status_message = "invalid stage"; break;
	    case shaderc_compilation_status_compilation_error: compilation_status_message = "compilation error"; break;
	    case shaderc_compilation_status_internal_error: compilation_status_message = "internal error"; break;
	    case shaderc_compilation_status_null_result_object: compilation_status_message = "null result object"; break;
	    case shaderc_compilation_status_invalid_assembly: compilation_status_message = "invalid assembly"; break;
	    case shaderc_compilation_status_validation_error: compilation_status_message = "validation error"; break;
	    case shaderc_compilation_status_transformation_error: compilation_status_message = "transformation error"; break;
	    case shaderc_compilation_status_configuration_error: compilation_status_message = "configuration error"; break;
	}

	char const* compilation_error_message = shaderc_result_get_error_message(compilation_result);

	std::size_t error_count = shaderc_result_get_num_errors(compilation_result);
	std::size_t warning_count = shaderc_result_get_num_warnings(compilation_result);

	// TODO: std::format
	Exceptions::Graphics::Shader_compiler::Compilation_error exception(
	    std::string("Shader compilation error") + 
	    std::string("\nCompilation status: ") + std::string(compilation_status_message) +
	    std::string("\nErrors: ") + std::to_string(error_count) + std::string(", warnings: ") + std::to_string(warning_count) +
	    std::string("\n\n") + std::string(compilation_error_message)
	);

	shaderc_result_release(compilation_result);
	
	// TODO: does this automatically move the object, like in a return statement?
	throw exception;
    }
}




