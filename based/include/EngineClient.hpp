#pragma once

#include <memory>

#include "Geometry.hpp"
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

	bool tick ();
	inline bool tickEvents ();
	inline void tickRender ();
	inline bool tickGui ();
	inline void tickFinish ();
public:
	EngineClient (Engine *_engine);
	void create_window (const std::string& title, const Size2D<int>& size);
	inline Window* window () { return sdl->window.get(); } 
};

}
