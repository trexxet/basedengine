#pragma once

#include <memory>

#include "Geometry.hpp"
#include "Resource.hpp"
#include "Window.hpp"

namespace Based {

class Engine;
class EngineClient {
	friend class Engine;
	Engine *engine = nullptr;

	struct SDL {
		SDL ();
		~SDL ();
		std::unique_ptr<Window> window {nullptr};
	};
	std::unique_ptr<SDL> sdl {nullptr};

	void tick ();
	inline void tickEvents ();
	inline void tickRender ();
	inline void tickFinish ();
public:
	ResourceManager resourceManager;

	EngineClient (Engine *_engine);

	void create_window (const std::string& title, const Vec2D<int>& size, const Window::Flags flags = Window::NO_FLAGS);
	inline Window* window () { return sdl->window.get(); } 
};

}
