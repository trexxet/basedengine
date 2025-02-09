#pragma once

#include <memory>
#include <string>

#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <SDL3/SDL_video.h>

#include "Geometry.hpp"
#include "NuklearCommon.h"

namespace Based {

class Window {
public:
	enum Flags {
		NO_FLAGS = 0,
		DISABLE_NUKLEAR = 1
	};
	Flags flags;

private:
	Vec2D<int> _size;
	Rect2D<int> _rect;
	GLfloat _aspect;
	glm::mat4 _ortho;

public:
	SDL_Window *sdlWindow;
	
	/// @brief Window size and corresponding rectangle
	const Vec2D<int>& size = _size;
	const Rect2D<int>& rect = _rect;
	/// @brief Window aspect (size.x / size.y)
	const GLfloat& aspect = _aspect;
	/// @brief Orthographic projection matrix for the window
	const glm::mat4& ortho = _ortho;
	/// @brief Pixel for Window center
	Vec2D<int> center () { return size / 2; }
	/// @brief Set window size and related members
	void resize (const Vec2D<int>& size);

	struct Nk {
		nk_context *ctx;
		Nk (Window& owner);
		~Nk ();
	};
	std::unique_ptr<Nk> nk {nullptr};
	inline nk_context* nk_ctx() { return nk->ctx; }

	void render ();

	Window (const std::string& title, const Vec2D<int>& _size, const Flags flags);
	~Window ();
};

}
