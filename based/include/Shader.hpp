#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <glad/gl.h>

#include "Resource.hpp"

namespace Based {

class Shader : public Resource {
	GLenum type;
	std::string source;
public:
	GLuint id = 0;

	Shader () = delete;
	Shader (GLenum type);
	Shader (GLenum type, std::string&& src);

	/// \note If path is empty, then it's assumed that the shader should compile from string set in constructor
	void load (const std::string& path = "") override final;
	/// Compile gl shader
	bool prepare () override final;
	void unload () override final;
};

using ShaderVec = std::vector<Shader *>;

class ShaderProgram : public Resource {
	ShaderVec units;
public:
	GLuint id = 0;

	ShaderProgram () = delete;
	ShaderProgram (ShaderVec&& units);

	/// Check if all shader units are compiled
	/// \note path is unused
	void load (const std::string& path = "") override final;
	/// Create & link gl shader program
	bool prepare () override final;
	void unload () override final;
};

namespace Default {

enum ShaderID {
	/// Vertex shader that forwards X Y vertex coordinates and S texture coordinate,
	/// and flips T texture coordinate
	S_2D_ForwardVert,
	/// Fragment shader that applies 2D sampler (texture) using S and T texture coordinates
	S_2D_SamplerFrag
};

enum ShaderProgramID {
	/// S_2D_ForwardVert, S_2D_SamplerFrag
	SP_2D_ForwardSampler
};

struct Shaders {
	std::unordered_map<ShaderID, Shader> shaders;
	std::unordered_map<ShaderProgramID, ShaderProgram> shaderPrograms;

	void init ();

	Shaders () = default;
	Shaders (const Shaders&) = delete;
	Shaders& operator= (const Shaders&) = delete;
	Shaders (Shaders&&) = delete;
	Shaders& operator= (Shaders&&) = delete;
	~Shaders ();
};

}

}
