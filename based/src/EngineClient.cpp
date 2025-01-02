#include "EngineClient.hpp"

#include <SDL3/SDL.h>

#include "Engine.hpp"
#include "Logger.hpp"

namespace Based {

EngineClient::_SDL::_SDL () {
	if (!SDL_Init (SDL_INIT_VIDEO))
		log.fatal("SDL init error: {}", SDL_GetError());
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	log.write("SDL initialized");
}

EngineClient::_SDL::~_SDL () {
	SDL_Quit();
}

EngineClient::EngineClient () {
	sdl = std::make_unique<_SDL>();
	engine.hasClient = true;
}

EngineClient::~EngineClient () {
	engine.hasClient = false;
}

void EngineClient::create_window (const std::string &title, int w, int h)
{
	window = std::make_unique<_Window> (title, w, h);
}

}
