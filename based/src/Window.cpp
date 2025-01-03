#include "Window.hpp"

#include "Logger.hpp"

namespace Based {

Window::Window(const std::string &title, int w, int h) {
	sdl_window  = SDL_CreateWindow (title.c_str(), w, h, SDL_WINDOW_OPENGL);
	if (!sdl_window)
		log.fatal ("Window {} could not be created!", title);
	log.write ("Window {} created", title);
}

Window::~Window() {
	if (sdl_window)
		SDL_DestroyWindow (sdl_window);
}

}
