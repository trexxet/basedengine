#define _BASED_DEFAULT_SHADERS_IMPLEMENT
#include "GL/Shader.hpp"
#undef _BASED_DEFAULT_SHADERS_IMPLEMENT

#include <glm/gtc/type_ptr.hpp>

#include "Logger.hpp"
#include "Util.hpp"
#include "GL/GL_Util.hpp"

namespace Based::GL {

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

Shader::Shader (GLenum type) : type (type) { }

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

	if (!check_glsl_status (id, glGetShaderiv, GL_COMPILE_STATUS)) [[unlikely]] {
		log.warn ("Failed to compile shader:\n{}", get_glsl_error (id, glGetShaderiv, glGetShaderInfoLog));
		return false;
	}

	source.clear();
	ready = true;
	return ready;
}

void Shader::unload () {
	source.clear();
	if (glIsShader (id))
		glDeleteShader (id);
	ready = false;
	loaded = false;
	id = 0;
}

Shader::~Shader() {
	if (id) unload();
}

std::unique_ptr<Shader> Shader::make_from_file (GLenum type, const std::string& path) {
	std::unique_ptr<Shader> shader = std::make_unique<Shader> (type);
	shader->load (path);
	if (!shader->prepare()) [[unlikely]]
		log.fatal ("Failed to prepare shader {}", path);
	return shader;
}

std::unique_ptr<Shader> Shader::make_from_string (GLenum type, std::string&& src) {
	std::unique_ptr<Shader> shader = std::make_unique<Shader> (type, std::forward<std::string>(src));
	shader->load ();
	if (!shader->prepare()) [[unlikely]]
		log.fatal ("Failed to prepare shader from source");
	return shader;
}

ShaderProgram::ShaderProgram (ShaderVec&& units, GLuint attributes)
	: units (units), attributes (attributes) { }

void ShaderProgram::load (const std::string& path) {
	for (const Shader* shader: units) {
		if (!shader->ready)
			log.warn ("Failed to instantiate shader program: shader units not compiled");
	}
}

bool ShaderProgram::prepare () {
	id = glCreateProgram();
	for (const Shader* shader: units)
		glAttachShader (id, shader->id);

	// Enabling Vertex Attribute Arrays before linking program
	// prevents shader recompilation based on GL state
	for (GLuint i = 0; i < attributes; i++) {
		glEnableVertexAttribArray (i);
		BASED_GL_CHECK ("Error enabling Vertex Attribute Array during shader program creation");
	}

	glLinkProgram (id);

	if (!check_glsl_status (id, glGetProgramiv, GL_LINK_STATUS)) [[unlikely]] {
		log.warn ("Failed to link shader program:\n{}", get_glsl_error (id, glGetProgramiv, glGetProgramInfoLog));
		return false;
	}
	
	ready = true;
	return ready;
}

void ShaderProgram::unload () {
	units.clear();
	if (glIsProgram (id))
		glDeleteProgram (id);
	ready = false;
	loaded = false;
	id = 0;
}

ShaderProgram::~ShaderProgram () {
	if (id) unload();
}

std::unique_ptr<ShaderProgram> ShaderProgram::make (ShaderVec&& units, GLuint attributes) {
	std::unique_ptr<ShaderProgram> sp = std::make_unique<ShaderProgram> (std::forward<ShaderVec> (units), attributes);
	sp->load ();
	if (!sp->prepare()) [[unlikely]]
		log.fatal ("Failed to prepare shader program!");
	return sp;
}

void ShaderProgram::use () {
	glUseProgram (id);
	BASED_GL_CHECK ("Error using shader program");
}

GLint ShaderProgram::get_uniform_location_from_gl (const GLchar *name) {
	GLint loc = glGetUniformLocation (id, name);
	if (loc < 0) [[unlikely]] {
		log.warn ("Can't get uniform {}", name);
		BASED_GL_CHECK ("Error getting uniform location");
	}
	return loc;
}

GLint ShaderProgram::get_uniform_location (const GLchar *name) {
	if (!useUniformLocationCache)
		return get_uniform_location_from_gl (name);
	
	std::string key (name);
	GLint loc;
	if (!uniformLocationCache.contains (key)) {
		loc = get_uniform_location_from_gl (name);
		uniformLocationCache[key] = loc;
	}
	else
		loc = uniformLocationCache[key];
	return loc;
}

void ShaderProgram::set_uniform (const GLchar *name, GLint value) {
	GLint loc = get_uniform_location (name);
	if (loc < 0) [[unlikely]] return;
	glUniform1i (loc, value);
	BASED_GL_CHECK ("Error setting uniform");
}

void ShaderProgram::set_uniform (const GLchar *name, const glm::vec2& value) {
	GLint loc = get_uniform_location (name);
	if (loc < 0) [[unlikely]] return;
	glUniform2fv (loc, 1, glm::value_ptr (value));
	BASED_GL_CHECK ("Error setting uniform");
}

void ShaderProgram::set_uniform (const GLchar *name, const glm::ivec2& value) {
	GLint loc = get_uniform_location (name);
	if (loc < 0) [[unlikely]] return;
	glUniform2iv (loc, 1, glm::value_ptr (value));
	BASED_GL_CHECK ("Error setting uniform");
}

void ShaderProgram::set_uniform (const GLchar *name, const glm::mat4& value) {
	GLint loc = get_uniform_location (name);
	if (loc < 0) [[unlikely]] return;
	glUniformMatrix4fv (loc, 1, GL_FALSE, glm::value_ptr (value));
	BASED_GL_CHECK ("Error setting uniform");
}

/*
 * Some built-in default shaders
 */
namespace Default {

Shaders shaders;

std::string src_2d_mvp_vert =
#include "shaders_builtin/2d_mvp.vert"
;
std::string src_2d_sampler_frag =
#include "shaders_builtin/2d_sampler.frag"
;

void Shaders::init () {
	shaders.emplace (std::piecewise_construct, std::forward_as_tuple (S_2D_MVPVert),
	                 std::forward_as_tuple (GL_VERTEX_SHADER, std::move (src_2d_mvp_vert)));
	shaders.emplace (std::piecewise_construct, std::forward_as_tuple (S_2D_SamplerFrag),
	                 std::forward_as_tuple (GL_FRAGMENT_SHADER, std::move (src_2d_sampler_frag)));

	for (auto& [id, shader] : shaders) {
		shader.load();
		shader.prepare();
		if (!shader.ready) [[unlikely]]
			log.fatal ("Failed to compile default shader");
	}

	ShaderVec SP_2D_MVPSampler_Units { &shaders.at (S_2D_MVPVert), &shaders.at (S_2D_SamplerFrag) };
	shaderPrograms.emplace (std::piecewise_construct, std::forward_as_tuple (SP_2D_MVPSampler),
	                        std::forward_as_tuple (std::move (SP_2D_MVPSampler_Units), 2));

	for (auto& [id, shaderProgram] : shaderPrograms) {
		shaderProgram.load();
		shaderProgram.prepare();
		if (!shaderProgram.ready) [[unlikely]]
			log.fatal ("Failed to link default shader program");
	}
}

}

}
