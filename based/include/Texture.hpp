#pragma once

#include <glad/gl.h>
#include <SDL3/SDL_surface.h>

#include "Resource.hpp"

namespace Based {

class Texture : public Resource {
	SDL_Surface *surface = nullptr;
public:
	GLuint id;

	void load (const std::string& path) override final;
	bool prepare () override final;
	void unload () override final;
};

}
