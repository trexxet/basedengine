#include "GL_Util.hpp"

#include "Logger.hpp"

namespace Based::GL {

void printGLErrorStack (const std::string& msg) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
		log.warn ("{}: 0x{:x}", msg, err);
}

}
