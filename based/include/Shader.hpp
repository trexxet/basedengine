#pragma once

#include <string>
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

	void load (const std::string& path) override final;
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

	void load (const std::string& path) override final;
	bool prepare () override final;
	void unload () override final;
};

}
