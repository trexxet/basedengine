#pragma once

#include <memory>

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

	bool tickEvents ();
	void tickRender ();
public:
	EngineClient (Engine *_engine);
	void create_window (const std::string& title, int w, int h);
};

}
