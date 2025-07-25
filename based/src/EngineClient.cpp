#include "EngineClient.hpp"

#include <SDL3/SDL.h>

#include "Engine.hpp"
#include "Logger.hpp"
#include "Scene.hpp"

namespace Based {

EngineClient::SDL::SDL () {
	if (!SDL_Init (SDL_INIT_VIDEO))
		log.fatal ("SDL init error: {}", SDL_GetError());
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	int sdlVersion = SDL_GetVersion();
	log.write("SDL {}.{}.{} initialized",
	          SDL_VERSIONNUM_MAJOR (sdlVersion),
	          SDL_VERSIONNUM_MINOR (sdlVersion),
	          SDL_VERSIONNUM_MICRO (sdlVersion));
}

EngineClient::SDL::~SDL () {
	if (window)
		window.reset();
	SDL_Quit();
}

EngineClient::EngineClient (Engine& engine) : engine(engine) {
	sdl = std::make_unique<SDL>();
}

WindowHandle EngineClient::create_window (const std::string &title, const Vec2D<int>& size, const Window::Flags flags)
{
	if (sdl->window)
		log.fatal ("Can't create multiple windows!");
	sdl->window = std::make_unique<Window> (*this, title, size, flags);
	return window();
}

void EngineClient::tick () {
	tick_events();
	engine.tick_update();
	window()->debugOverlay.update();
	tick_render();
}

inline void EngineClient::tick_events () {
	SDL_Event event;

	while (SDL_PollEvent (&event)) {
		switch (event.type) {
			[[unlikely]]
			case SDL_EVENT_QUIT:
				engine.stop();
				break;
			default:
				break;
		}
		engine.sceneManager.handle_event (&event);
		window()->debugOverlay.handle_event (&event);
	}
}

inline void EngineClient::tick_render () {
	glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	engine.sceneManager.render();

	window()->debugOverlay.render();
	window()->render();
}

}
