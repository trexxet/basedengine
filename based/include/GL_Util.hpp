#pragma once

#include <string>

#include <glad/gl.h>

namespace Based::GL {

#ifdef BASED_ENABLE_GL_CHECK
#define BASED_GL_CHECK(msg) Based::GL::printGLErrorStack (msg)
#else
#define BASED_GL_CHECK(msg)
#endif

void printGLErrorStack (const std::string& msg);

}
