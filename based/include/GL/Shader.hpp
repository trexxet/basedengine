#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <glad/gl.h>

#include "ClassHelper.hpp"
#include "Resource.hpp"

namespace Based::GL {

class Shader : public Resource {
	GLenum type;
	std::string source;
public:
	GLuint id = 0;

	Shader () = delete;
	Shader (GLenum type);
	Shader (GLenum type, std::string&& src);

	/// @note If path is empty, then it's assumed that the shader should compile from string set in constructor
	void load (const std::string& path = "") override final;
	/// @brief Compile gl shader
	bool prepare () override final;
	void unload () override final;

	~Shader () override final;
};

using ShaderVec = std::vector<Shader *>;

class ShaderProgram : public Resource {
	ShaderVec units;
public:
	GLuint id = 0, attributes = 0;

	ShaderProgram () = delete;
	/// @brief Constructor for ShaderProgram
	/// @param units Vector of Shader* pointing to the corresponding Shader units
	/// @param attributes Number of generic vertex attributes
	ShaderProgram (ShaderVec&& units, GLuint attributes);

	/// @brief Check if all shader units are compiled
	/// @note path is unused
	void load (const std::string& path = "") override final;
	/// @brief Create & link gl shader program
	bool prepare () override final;
	void unload () override final;

	~ShaderProgram () override final;

	/// @brief Use the shader program
	/// @note Shader program readiness is not checked
	void use ();
	/// @brief Set uniform
	/// @param name Uniform name
	/// @param value Uniform value
	void setUniform (const GLchar *name, GLint value);
};

namespace Default {

enum ShaderID {
	/// @brief Vertex shader that forwards X Y vertex coordinates and S texture coordinate,
	/// and flips T texture coordinate
	S_2D_ForwardVert,
	/// @brief Fragment shader that applies 2D sampler (texture) using S and T texture coordinates
	S_2D_SamplerFrag
};

enum ShaderProgramID {
	/// @brief S_2D_ForwardVert, S_2D_SamplerFrag
	SP_2D_ForwardSampler
};

struct Shaders {
	std::unordered_map<ShaderID, Shader> shaders;
	std::unordered_map<ShaderProgramID, ShaderProgram> shaderPrograms;

	void init ();

	inline Shader& operator() (ShaderID id) { return shaders.at (id); }
	inline ShaderProgram& operator[] (ShaderProgramID id) { return shaderPrograms.at (id); }

	Shaders () = default;
	BASED_CLASS_NO_COPY_MOVE (Shaders);
};

}

}
