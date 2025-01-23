#pragma once

#include <memory>

#include <glad/gl.h>
#include <SDL3/SDL_surface.h>

#include "Geometry.hpp"
#include "Resource.hpp"

namespace Based::GL {

class Texture : public Resource {
	SDL_Surface *surface = nullptr;
public:
	GLuint id, unit;
	Size2D<int> size;
	/// @brief {0, 0, size.x, size.y}
	Rect2D<int> rect;
	/// @brief Get {0, 0, 1, 1} rect in GL texture coordinates
	constexpr static Rect2D<GLfloat> full () { return {0.f, 0.f, 1.f, 1.f}; }
	/// @brief Get {0.5f, 0.5f} point in GL texture coordinates
	constexpr static Point2D<GLfloat> center () { return {0.5f, 0.5f}; }

	Texture () = delete;
	Texture (GLuint unit);

	void load (const std::string& path) override final;
	bool prepare () override final;
	void unload () override final;

	~Texture () override final;

	/// @brief Create, load, prepare and optionally bind texture
	static std::unique_ptr<Texture> make (const std::string& path, GLuint unit);
};

}
