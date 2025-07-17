#pragma once

#include <memory>

#include <glad/gl.h>
#include <SDL3/SDL_surface.h>

#include "Defs.hpp"
#include "Geometry.hpp"
#include "Resource.hpp"

namespace Based::GL {

class BASED_API Texture : public Resource {
	SDL_Surface *surface = nullptr;
public:
	GLuint id;
	Vec2D<int> size;
	/// @brief {0, 0, size.x, size.y}
	Rect2D<int> rect;
	/// @brief Get {0, 0, 1, 1} rect in GL texture coordinates
	constexpr static Rect2D<GLfloat> full () { return { 0.f, 0.f, 1.f, 1.f }; }
	/// @brief Get {0.5f, 0.5f} point in GL texture coordinates
	constexpr static Vec2D<GLfloat> center () { return { 0.5f, 0.5f }; }

	Texture () {}

	void load (const std::string& path) override final;
	bool prepare () override final;
	void unload () override final;

	~Texture () override final;

	/// @brief Create, load and prepare texture
	static std::unique_ptr<Texture> make (const std::string& path);

	/// @brief Bind texture to the specified texture unit, or to the currently active one
	/// @param unit Texture unit (-1 if should use currently active unit)
	/// @note Texture readiness is not checked
	void use (GLint unit = -1);
};

}
