#include "Shader.hpp"

#include "GL_Util.hpp"
#include "Logger.hpp"
#include "Util.hpp"

namespace Based {

using _gl_getivproc_t = void (*)(GLuint id, GLenum pname, GLint *params);
using _gl_getinfoproc_t = void (*)(GLuint id, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

static bool check_glsl_status (GLuint id, _gl_getivproc_t _gl_getivproc, GLenum statusName) {
	GLint ok;
	_gl_getivproc (id, statusName, &ok);
	return static_cast<bool>(ok);
}

static std::string get_glsl_error(GLuint id, _gl_getivproc_t _gl_getivproc, _gl_getinfoproc_t _gl_getinfoproc) {
	std::string errMsg;
	GLint errLen;
	_gl_getivproc (id, GL_INFO_LOG_LENGTH, &errLen);
	errMsg.resize (errLen);
	_gl_getinfoproc (id, errLen, NULL, &errMsg[0]);
	errMsg.erase (errMsg.find_last_not_of ("\n") - 1);
	return errMsg;
}

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

	if (!check_glsl_status(id, glGetShaderiv, GL_COMPILE_STATUS)) [[unlikely]] {
		log.warn ("Failed to compile shader:\n{}", get_glsl_error(id, glGetShaderiv, glGetShaderInfoLog));
		return false;
	}

	source.clear();
	ready = true;
	return ready;
}

void Shader::unload () {
	if (glIsShader (id))
		glDeleteShader (id);
	ready = false;
	loaded = false;
	id = 0;
}

ShaderProgram::ShaderProgram (ShaderVec&& units) : units(units) { }

void ShaderProgram::load (const std::string& path) {
	for (const Shader* shader: units) {
		if (!shader->ready)
			log.warn ("Failed to instantiate shader program: shader units not compiled");
	}
}

bool ShaderProgram::prepare () {
	id = glCreateProgram ();
	for (const Shader* shader: units)
		glAttachShader (id, shader->id);
	glLinkProgram (id);

	if (!check_glsl_status(id, glGetProgramiv, GL_LINK_STATUS)) [[unlikely]] {
		log.warn ("Failed to link shader program:\n{}", get_glsl_error(id, glGetProgramiv, glGetProgramInfoLog));
		return false;
	}
	
	ready = true;
	return ready;
}

void ShaderProgram::unload () {
	if (glIsProgram (id))
		glDeleteProgram (id);
	ready = false;
	loaded = false;
	id = 0;
}

void ShaderProgram::use () {
	glUseProgram (id);
	BASED_GL_CHECK ("Error using shader program");
}

// todo: uniform location cache
void ShaderProgram::setUniform (const GLchar *name, GLint value) {
	GLint loc = glGetUniformLocation (id, name);
	BASED_GL_CHECK ("Error getting uniform location");
	glUniform1i (loc, value);
	BASED_GL_CHECK ("Error setting uniform");
}

/*
 * Some built-in default shaders
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
		if (!shader.ready) [[unlikely]]
			log.fatal ("Failed to compile default shader");
	}

	ShaderVec SP_2D_ForwardSampler_Units { &shaders.at(S_2D_ForwardVert), &shaders.at(S_2D_SamplerFrag) };
	shaderPrograms.try_emplace (SP_2D_ForwardSampler, ShaderProgram (std::move(SP_2D_ForwardSampler_Units)));

	for (auto& [id, shaderProgram] : shaderPrograms) {
		shaderProgram.load();
		shaderProgram.prepare();
		if (!shaderProgram.ready) [[unlikely]]
			log.fatal ("Failed to link default shader program");
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
