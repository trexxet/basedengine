#pragma once

#include <memory>
#include <string>

#include <glad/gl.h>
#include <glm/vec2.hpp>
#include <SDL3/SDL_video.h>

#include "Geometry.hpp"
#include "NuklearCommon.h"

namespace Based {

struct Window {
	SDL_Window *sdlWindow;
	Size2D<int> size;
	Rect2D<int> rect;

	struct Nk {
		nk_context *ctx;
		Nk (Window& owner);
		~Nk ();
	};
	std::unique_ptr<Nk> nk {nullptr};
	inline nk_context* nk_ctx() { return nk->ctx; }

	void render ();

	Window (const std::string& title, const Size2D<int>& _size);
	~Window ();

	// Coordinate space transformations
	inline glm::vec2 Window2GL (const Point2D<int>& p) const {
		return {
			(2.f * p.x) / size.x - 1.f,
			(2.f * p.y) / size.y - 1.f
		};
	}
	inline Point2D<int> GL2Window (const glm::vec2& p) const {
		return {
			(int)((p.x + 1) * size.x * 0.5f),
			(int)((p.y + 1) * size.y * 0.5f)
		};
	}
	inline Rect2D<float> Window2GL (const Rect2D<int>& r) const {
		return {
			(2.f * r.x) / size.x - 1.f,
			(2.f * r.y) / size.y - 1.f,
			(2.f * r.width) / size.x - 1.f,
			(2.f * r.height) / size.y - 1.f
		};
	}
	inline Rect2D<int> GL2Window (const Rect2D<float>& r) const {
		return {
			(int)((r.x + 1) * size.x * 0.5f),
			(int)((r.y + 1) * size.y * 0.5f),
			(int)((r.width + 1) * size.x * 0.5f),
			(int)((r.height + 1) * size.y * 0.5f)
		};
	}
};

}
