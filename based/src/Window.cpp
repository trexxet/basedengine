#include "Window.hpp"

#include "Logger.hpp"

namespace Based {

_Window::_Window(const std::string &title, int w, int h) {
	sdl_window  = SDL_CreateWindow (title.c_str(), w, h, SDL_WINDOW_OPENGL);
	if (!sdl_window)
		log.fatal ("Window {} could not be created!", title);
	log.write ("Window {} created", title);
}

_Window::~_Window() {
	if (sdl_window)
		SDL_DestroyWindow (sdl_window);
}

}