#include "Window.hpp"

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Logger.hpp"

namespace Based {

Window::Window (const std::string &title, const Vec2D<int>& size, const Flags flags) : flags (flags) {
	sdlWindow  = SDL_CreateWindow (title.c_str(), size.x, size.y, SDL_WINDOW_OPENGL);
	if (!sdlWindow)
		log.fatal ("Window \"{}\" could not be created!", title);
	log.write ("Window \"{}\" created", title);
	resize (size);

	if (!SDL_GL_CreateContext(sdlWindow))
		log.fatal ("Failed to create OpenGL context!");
	log.write ("SDL GL context created");

	if (!SDL_GL_SetSwapInterval (1))
		log.warn ("Failed to enable VSync!");
	else
		log.write ("VSync enabled");

	log.write ("OpenGL loader: glad {}", GLAD_GENERATOR_VERSION);
	if (!gladLoadGL ((GLADloadfunc) SDL_GL_GetProcAddress))
		log.fatal ("Failed to load OpenGL!");
	log.write_inc ("OpenGL loaded: {}", (const char*) glGetString (GL_VERSION));
	log.write_dec ("Render device: {}", (const char*) glGetString (GL_RENDERER));

	glClearColor (0, 0, 0, 1);
	if (glGetError () != GL_NO_ERROR)
		log.fatal ("Failed to use OpenGL!");

	if (flags & Flags::DISABLE_RML)
		log.write ("RML initialization skipped!");
	else
		rml = std::make_unique<RML::Interface> (this);
}

Window::~Window () {
	// TODO: Is SDL_GL_DeleteContext needed here?
	if (sdlWindow)
		SDL_DestroyWindow (sdlWindow);
}

void Window::render () {
	SDL_GL_SwapWindow(sdlWindow);
}

void Window::resize (const Vec2D<int>& size) {
	_size = size;
	_rect = {size};
	_aspect = (GLfloat) size.x / size.y;
	_ortho = glm::ortho<GLfloat> (0, size.x, size.y, 0);

	// TODO: Handle resize for RML
}

}
