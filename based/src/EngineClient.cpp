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
	SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
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

EngineClient::EngineClient (Engine *_engine) : engine(_engine) {
	sdl = std::make_unique<SDL>();
}

void EngineClient::create_window (const std::string &title, const Size2D<int>& size)
{
	if (sdl->window)
		log.fatal ("Can't create multiple windows!");
	sdl->window = std::make_unique<Window> (title, size);
}

bool EngineClient::tick () {
	bool keepRunning = true;

	keepRunning = tickEvents();

	if (keepRunning)
		keepRunning = engine->tickUpdate();

	if (keepRunning) {
		tickRender();
		keepRunning = tickGui();
	}

	tickFinish();

	return keepRunning;
}

inline bool EngineClient::tickEvents () {
	bool keepRunning = true;
	SDL_Event event;
	nk_context *nk_ctx = sdl->window->nk->ctx;

	nk_input_begin (nk_ctx);
	while (SDL_PollEvent (&event)) {
		switch (event.type) {
			[[unlikely]]
			case SDL_EVENT_QUIT:
				keepRunning = false;
				break;
			default:
				nk_sdl_handle_event (&event);
		}
		keepRunning &= engine->sceneManager.handle_events (&event);
	}
	nk_input_end (nk_ctx);

	return keepRunning;
}

inline void EngineClient::tickRender () {
	glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	engine->sceneManager.render (window());
}

inline bool EngineClient::tickGui () {
	return engine->sceneManager.gui (window());
}

inline void EngineClient::tickFinish () {
	sdl->window->render();
}

}
