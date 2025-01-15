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
	GLuint id;

	Shader () = delete;
	Shader (GLenum type);
	Shader (GLenum type, std::string&& src);

	void load (const std::string& path = "") override final;
	bool prepare () override final;
	void unload () override final;
};

using ShaderVec = std::vector<Shader *>;

class ShaderProgram : public Resource {
	ShaderVec units;
public:
	GLuint id;

	ShaderProgram () = delete;
	ShaderProgram (ShaderVec&& units);

	void load (const std::string& path = "") override final;
	bool prepare () override final;
	void unload () override final;
};

namespace Default {

enum ShaderID {
	S_2D_ForwardVert,
	S_2D_SamplerFrag
};

enum ShaderProgramID {
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
