#pragma once

#include <string>

#include <SDL3/SDL_video.h>

namespace Based {

struct Window {
	SDL_Window *sdl_window;

	Window (const std::string& title, int w, int h);
	~Window ();
};

}
