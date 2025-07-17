#pragma once

#include <memory>

#include "Defs.hpp"
#include "Geometry.hpp"
#include "Resource.hpp"
#include "Window.hpp"

namespace Based {

class Engine;
class BASED_API EngineClient {
	friend class Engine;

	struct BASED_API SDL {
		SDL ();
		~SDL ();
		std::unique_ptr<Window> window {nullptr};
		BASED_CLASS_NO_COPY_MOVE (SDL);
	};
	std::unique_ptr<SDL> sdl {nullptr};

	void tick ();
	inline void tick_events ();
	inline void tick_render ();
public:
	Engine& engine;
	ResourceManager resourceManager;

	EngineClient (Engine& engine);

	WindowHandle create_window (const std::string& title, const Vec2D<int>& size, const Window::Flags flags = Window::NO_FLAGS);
	inline WindowHandle window () { return sdl->window.get(); }

	BASED_CLASS_NO_COPY_MOVE (EngineClient);
};

}
