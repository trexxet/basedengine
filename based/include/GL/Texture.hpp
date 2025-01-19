#pragma once

#include <glad/gl.h>
#include <glm/vec2.hpp>
#include <SDL3/SDL_surface.h>

#include "Geometry.hpp"
#include "Resource.hpp"

namespace Based::GL {

class Texture : public Resource {
	SDL_Surface *surface = nullptr;
public:
	GLuint id;
	Size2D<int> size;

	void load (const std::string& path) override final;
	bool prepare () override final;
	void unload () override final;

	~Texture () override final;

	/// @brief Bind texture to texture unit
	/// @param unit Texture unit number
	/// @note Texture readiness is not checked
	void bind (GLuint unit);

	// Coordinate space transformations
	inline glm::vec2 Tex2GL (const Point2D<int>& p) const {
		return {
			(GLfloat) p.x / size.x,
			(GLfloat) p.y / size.y
		};
	}
	inline Point2D<int> GL2Tex (const glm::vec2& p) const {
		return {
			(int) (p.x * size.x),
			(int) (p.y * size.y)
		};
	}
	inline Rect2D<float> Tex2GL (const Rect2D<int>& r) const {
		return {
			(GLfloat) r.x / size.x,
			(GLfloat) r.y / size.y,
			(GLfloat) r.width / size.x,
			(GLfloat) r.height / size.y
		};
	}
	inline Rect2D<int> GL2Tex (const Rect2D<float>& r) const {
		return {
			(int) (r.x * size.x),
			(int) (r.y * size.y),
			(int) (r.width * size.x),
			(int) (r.height * size.y)
		};
	}
};

}
