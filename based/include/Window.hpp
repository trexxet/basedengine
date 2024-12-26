#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>

namespace Based {

class _Window {
	SDL_Window *sdl_window;
public:
	_Window (const std::string& title, int w, int h);
	~_Window ();
};

using Window = std::unique_ptr<_Window>;

}
