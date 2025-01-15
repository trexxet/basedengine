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
	ready = false;
}

}
