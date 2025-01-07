#pragma once

#include "NuklearAuxiliaries.hpp"
#include "Scene.hpp"

class SceneMain : public Based::Scene {
public:
	SceneMain (Based::Window* window, Based::Lua::File& conf) { }

	bool handle_events (SDL_Event *event) override final {
		return true;
	}

	bool update () override final {
		return true;
	}

	void render (Based::Window* window) override final { }
};

