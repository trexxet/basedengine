#include "Window.hpp"

#include <glad/gl.h>

#include "Logger.hpp"

namespace Based {

Window::Window (const std::string &title, const Size2D<int>& _size) : size (_size) {
	sdlWindow  = SDL_CreateWindow (title.c_str(), size.width, size.height, SDL_WINDOW_OPENGL);
	if (!sdlWindow)
		log.fatal ("Window \"{}\" could not be created!", title);
	log.write ("Window \"{}\" created", title);

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
	glEnable (GL_DEPTH_TEST);
	if (glGetError () != GL_NO_ERROR)
		log.fatal ("Failed to use OpenGL!");

	nk = std::make_unique<Nk>(*this);
}

Window::~Window () {
	// TODO: Is SDL_GL_DeleteContext needed here?
	if (sdlWindow)
		SDL_DestroyWindow (sdlWindow);
}

Window::Nk::Nk (Window& owner) {
	if (!owner.sdlWindow)
		log.fatal ("Failed to initialize Nuklear: window is not created yet!");
	ctx = nk_sdl_init (owner.sdlWindow);
	nk_font_atlas *atlas;
	nk_sdl_font_stash_begin (&atlas);
	nk_sdl_font_stash_end ();
	log.write ("Nuklear {} initialized", NK_VERSION);
}

Window::Nk::~Nk () {
	nk_sdl_shutdown ();
}

void Window::render () {
	nk_sdl_render (NK_ANTI_ALIASING_ON, NK_MAX_VERTEX_MEMORY, NK_MAX_ELEMENT_MEMORY);
	glEnable (GL_DEPTH_TEST);
	SDL_GL_SwapWindow(sdlWindow);
}

}
