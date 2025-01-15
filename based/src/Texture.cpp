#include "Texture.hpp"

#include <SDL3_image/SDL_image.h>

#include "Logger.hpp"

namespace Based {

void Texture::load (const std::string& path) {
	surface = IMG_Load (path.c_str());
	if (!surface)
		log.fatal ("Can't load texture {}: {}", path, SDL_GetError());

	log.write ("Loaded texture: {}", path);
	loaded = true;
}

bool Texture::prepare () {
	if (ready || !loaded)
		return ready;

	const SDL_PixelFormatDetails *pfd = SDL_GetPixelFormatDetails(surface->format);
	GLenum mode = (pfd->bytes_per_pixel == 4) ? GL_RGBA : GL_RGB;

	glGenTextures (1, &id);
	glBindTexture (GL_TEXTURE_2D, id);
	glTexImage2D (GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_DestroySurface (surface);
	surface = nullptr;
	
	ready = true;
	return ready;
}

void Texture::unload () {
	if (!ready) return;
	glDeleteTextures (1, &id);
	ready = false;
	loaded = false;
}

}
