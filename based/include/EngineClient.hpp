#pragma once

#include <memory>

#include "Geometry.hpp"
#include "Resource.hpp"
#include "Window.hpp"
#include "GL/Shader.hpp"

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
	inline void tickGui ();
	inline void tickFinish ();
public:
	ResourceManager resourceManager;

	EngineClient (Engine *_engine);

	void create_window (const std::string& title, const Size2D<int>& size);
	inline Window* window () { return sdl->window.get(); } 

	GL::Default::Shaders defaultShaders;
};

}
