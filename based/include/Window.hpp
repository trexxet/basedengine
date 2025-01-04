#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL_video.h>

#include "NuklearCommon.h"

namespace Based {

struct Window {
	SDL_Window *sdlWindow;

	struct Nk {
		nk_context *ctx;
		Nk (Window& owner);
		~Nk ();
	};
	std::unique_ptr<Nk> nk {nullptr};

	void render ();

	Window (const std::string& title, int w, int h);
	~Window ();
};

}
