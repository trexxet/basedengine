#include "Window.hpp"

#include <glad/gl.h>

#include "Logger.hpp"

namespace Based {

Window::Window(const std::string &title, int w, int h) {
	sdl_window  = SDL_CreateWindow (title.c_str(), w, h, SDL_WINDOW_OPENGL);
	if (!sdl_window)
		log.fatal ("Window \"{}\" could not be created!", title);
	log.write ("Window \"{}\" created", title);

	if (!SDL_GL_CreateContext(sdl_window))
		log.fatal ("Failed to create OpenGL context!");
	log.write ("SDL GL context created");

	log.write ("OpenGL loader: glad {}", GLAD_GENERATOR_VERSION);
	if (!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress))
		log.fatal ("Failed to load OpenGL!");
	log.write_inc ("OpenGL loaded: {}", (const char*) glGetString (GL_VERSION));
	log.write_dec ("Render device: {}", (const char*) glGetString (GL_RENDERER));

	glClearColor (0, 0, 0, 1);
	glEnable (GL_DEPTH_TEST);
	if (glGetError () != GL_NO_ERROR)
		log.fatal ("Failed to use OpenGL!");
}

Window::~Window() {
	// TODO: Is SDL_GL_DeleteContext needed here?
	if (sdl_window)
		SDL_DestroyWindow (sdl_window);
}

}
