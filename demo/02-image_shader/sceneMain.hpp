#pragma once

#include "NuklearAuxiliaries.hpp"
#include "Scene.hpp"

class SceneMain : public Based::Scene {
public:
	SceneMain (Based::Engine* engine, Based::Lua::File& conf) : Based::Scene(engine)  { }

	bool handle_events (SDL_Event *event) override final {
		return true;
	}

	bool update () override final {
		return true;
	}

	void render () override final { }

	bool gui () override final {
		return true;
	}
};
