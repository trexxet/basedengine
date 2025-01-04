#include "sceneMain.hpp"

#include "Logger.hpp"

bool SceneMain::handle_events (SDL_Event *event) {
	return true;
}

bool SceneMain::update () {
	return true;
}

void SceneMain::render (Based::Window* window) {
	nk_context *nk_ctx = window->nk->ctx;

	if (nk_begin (nk_ctx, "Demo 01", nk_rect(50, 50, 230, 250),
	    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)) {
		
	}
	nk_end (nk_ctx);
}
