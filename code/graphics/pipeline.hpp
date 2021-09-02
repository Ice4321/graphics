#ifndef INCLUDED_GRAPHICS_PIPELINE_HPP
#define INCLUDED_GRAPHICS_PIPELINE_HPP

#include"graphics/shader_module.hpp"

namespace Graphics {
    class Pipeline {
    public:
	// TODO: make this variadic
	Pipeline(Shader_module& _vertex_shader, Shader_module& _fragment_shader);

	~Pipeline();

	Pipeline(Pipeline const&) = delete;
	Pipeline& operator=(Pipeline const&) = delete;

    private:

}

#endif
