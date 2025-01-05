#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL_video.h>

#include "Geometry.hpp"
#include "NuklearCommon.h"

namespace Based {

struct Window {
	SDL_Window *sdlWindow;
	Size2D<int> size {0, 0};

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
};

}
