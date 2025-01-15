#include "Shader.hpp"

#include "Logger.hpp"
#include "Util.hpp"

namespace Based {

Shader::Shader (GLenum type) : type(type) { }

Shader::Shader (GLenum type, std::string&& src) : Shader(type) {
	source = src;
	loaded = true;
}

void Shader::load (const std::string& path) {
	if (loaded && path.empty()) return;

	source = load_file_to_string (path);
	if (source.empty()) [[unlikely]]
		log.fatal ("Can't load shader {}", path);

	loaded = true;
}

bool Shader::prepare () {
	if (ready || !loaded)
		return ready;

	id = glCreateShader (type);
	const GLchar* src = source.c_str();
	glShaderSource (id, 1, &src, NULL);
	glCompileShader (id);

	GLint ok;
	glGetShaderiv (id, GL_COMPILE_STATUS, &ok);
	if (!ok) [[unlikely]] {
		GLint errLen;
		glGetShaderiv (id, GL_INFO_LOG_LENGTH, &errLen);
		std::string errMsg;
		errMsg.resize (errLen);
		glGetShaderInfoLog (id, errLen, NULL, &errMsg[0]);
		log.warn ("Failed to compile shader:\n{}", errMsg);
		return false;
	}

	source.clear();
	ready = true;
	return ready;
}

void Shader::unload () {
	if (!ready) return;
	glDeleteShader (id);
	ready = false;
	loaded = false;
}

ShaderProgram::ShaderProgram (ShaderVec&& units) : units(units) { }

void ShaderProgram::load (const std::string& path) {
	for (const Shader* shader: units) {
		if (!shader->ready)
			log.fatal ("Failed to instantiate shader program: not all shaders are ready");
	}
}

bool ShaderProgram::prepare () {
	id = glCreateProgram ();
	for (const Shader* shader: units)
		glAttachShader (id, shader->id);
	glLinkProgram (id);

	GLint ok;
	glGetProgramiv (id, GL_LINK_STATUS, &ok);
	if (!ok) [[unlikely]] {
		GLint errLen;
		glGetProgramiv (id, GL_INFO_LOG_LENGTH, &errLen);
		std::string errMsg;
		errMsg.resize (errLen);
		glGetProgramInfoLog (id, errLen, NULL, &errMsg[0]);
		log.warn ("Failed to link shader program:\n{}", errMsg);
		return false;
	}
	
	ready = true;
	return ready;
}

void ShaderProgram::unload () {
	if (!ready) return;
	glDeleteProgram(id);
	ready = false;
	loaded = false;
}

/*
 *
 * Some built-in default shaders
 *
 */
namespace Default {

	std::string src_2d_forward_vert =
#include "shaders_builtin/2d_forward.vert"
;
	std::string src_2d_sampler_frag =
#include "shaders_builtin/2d_sampler.frag"
;

void Shaders::init () {
	shaders.try_emplace (S_2D_ForwardVert, Shader(GL_VERTEX_SHADER, std::move(src_2d_forward_vert)));
	shaders.try_emplace (S_2D_SamplerFrag, Shader(GL_FRAGMENT_SHADER, std::move(src_2d_sampler_frag)));

	for (auto& [id, shader] : shaders) {
		shader.load();
		shader.prepare();
	}

	ShaderVec SP_2D_ForwardSampler_Units { &shaders.at(S_2D_ForwardVert), &shaders.at(S_2D_SamplerFrag) };
	shaderPrograms.try_emplace (SP_2D_ForwardSampler, ShaderProgram (std::move(SP_2D_ForwardSampler_Units)));

	for (auto& [id, shaderProgram] : shaderPrograms) {
		shaderProgram.load();
		shaderProgram.prepare();
	}
}

Shaders::~Shaders () {
	for (auto& [id, shaderProgram] : shaderPrograms)
		shaderProgram.unload();
	for (auto& [id, shader] : shaders)
		shader.unload();
}

}

}
