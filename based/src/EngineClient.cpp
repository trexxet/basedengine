#include "EngineClient.hpp"

#include <SDL3/SDL.h>

#include "Engine.hpp"
#include "Logger.hpp"
#include "Scene.hpp"

namespace Based {

EngineClient::SDL::SDL () {
	if (!SDL_Init (SDL_INIT_VIDEO))
		log.fatal("SDL init error: {}", SDL_GetError());
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	log.write("SDL initialized");
}

EngineClient::SDL::~SDL () {
	if (window)
		window.reset();
	SDL_Quit();
}

EngineClient::EngineClient (Engine *_engine) : engine(_engine) {
	sdl = std::make_unique<SDL>();
}

void EngineClient::create_window (const std::string &title, int w, int h)
{
	if (sdl->window)
		log.fatal ("Can't create multiple windows!");
	sdl->window = std::make_unique<Window> (title, w, h);
}

bool EngineClient::tickEvents () {
	bool result = true;

	SDL_Event event;
	while (SDL_PollEvent (&event)) {
		switch (event.type) {
			[[unlikely]]
			case SDL_EVENT_QUIT:
				result = false;
				break;
			default: break;
		}
		result &= engine->sceneManager.handle_events (&event);
	}

	return result;
}

void EngineClient::tickRender () {
	engine->sceneManager.render();
}

}
