#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <glad/gl.h>
#include <glm/mat4x4.hpp>

#include "Defs.hpp"
#include "Resource.hpp"

namespace Based::GL {

class BASED_API Shader : public Resource {
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

	/// @brief Create, load, and prepare Shader from file
	static std::unique_ptr<Shader> make_from_file (GLenum type, const std::string& path);
	/// @brief Create, load, and prepare Shader from source string
	static std::unique_ptr<Shader> make_from_string (GLenum type, std::string&& src);
};

using ShaderVec = std::vector<Shader *>;

class BASED_API ShaderProgram : public Resource {
	ShaderVec units;

	GLint get_uniform_location_from_gl (const GLchar *name);
	GLint get_uniform_location (const GLchar *name);
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

	/// @brief Create, load, and prepare ShaderProgram
	static std::unique_ptr<ShaderProgram> make (ShaderVec&& units, GLuint attributes);

	/// @brief Use the shader program
	/// @note Shader program readiness is not checked
	void use ();

	std::unordered_map<std::string, GLint> uniformLocationCache;
	bool useUniformLocationCache = true;

	/// @brief Set uniform
	/// @param name Uniform name
	/// @param value Uniform value
	void set_uniform (const GLchar *name, GLint value);
	void set_uniform (const GLchar *name, const glm::vec2& value);
	void set_uniform (const GLchar *name, const glm::ivec2& value);
	void set_uniform (const GLchar *name, const glm::mat4& value);
};

namespace Default {

enum ShaderID {
	/// @brief Vertex shader that applies MVP to (X, Y) and passes (S, 1 - T) further
	S_2D_MVPVert,
	/// @brief Fragment shader that applies 2D sampler (texture) using S and T texture coordinates
	S_2D_SamplerFrag
};

enum ShaderProgramID {
	/// @brief S_2D_MVPVert, S_2D_SamplerFrag
	SP_2D_MVPSampler
};

struct BASED_API Shaders {
	std::unordered_map<ShaderID, Shader> shaders;
	std::unordered_map<ShaderProgramID, ShaderProgram> shaderPrograms;

	void init ();

	inline Shader& operator() (ShaderID id) { return shaders.at (id); }
	inline ShaderProgram& operator[] (ShaderProgramID id) { return shaderPrograms.at (id); }

	Shaders () = default;
	BASED_CLASS_NO_COPY_MOVE (Shaders);
};

#ifndef _BASED_DEFAULT_SHADERS_IMPLEMENT
extern Shaders shaders;
#endif

}

}
