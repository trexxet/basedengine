#pragma once

#include <memory>

#include "Window.hpp"

namespace Based {

class EngineClient {
	struct _SDL {
		_SDL ();
		~_SDL ();
	};
	using SDL = std::unique_ptr<_SDL>;
	SDL sdl;

	Window window;
public:
	EngineClient ();
	~EngineClient ();

	void create_window (const std::string& title, int w, int h);
};

}
