#ifndef INCLUDED_GRAPHICS_SHADER_HPP
#define INCLUDED_GRAPHICS_SHADER_HPP

namespace Graphics {
    class Shader {
    public:
	Shader();

	~Shader();
	
	Shader(Shader const&) = delete;
	Shader& operator=(Shader const&) = delete;
    };
};

#endif
