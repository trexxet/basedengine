#pragma once

#include <memory>

#include <glad/gl.h>
#include <glm/vec2.hpp>
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
	constexpr static Rect2D<GLfloat> Full () { return {0.f, 0.f, 1.f, 1.f}; }
	/// @brief Get {0.5f, 0.5f} point in GL texture coordinates
	constexpr static Point2D<GLfloat> Center () { return {0.5f, 0.5f}; }

	Texture () = delete;
	Texture (GLuint unit);

	void load (const std::string& path) override final;
	bool prepare () override final;
	void unload () override final;

	~Texture () override final;

	/// @brief Create, load, prepare and optionally bind texture
	static std::unique_ptr<Texture> make (const std::string& path, GLuint unit);

	// Coordinate space transformations
	// todo: vectorize
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
			(GLfloat) r.w / size.x,
			(GLfloat) r.h / size.y
		};
	}
	inline Rect2D<int> GL2Tex (const Rect2D<float>& r) const {
		return {
			(int) (r.x * size.x),
			(int) (r.y * size.y),
			(int) (r.w * size.x),
			(int) (r.h * size.y)
		};
	}
	inline static GLfloat toNDC (const GLfloat x) {
		return 2.f * x - 1.f;
	}
	inline static GLfloat fromNDC (const GLfloat x) {
		return (x + 1.f) * 0.5f;
	}
};

}
