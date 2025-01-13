#pragma once

#include "NuklearAuxiliaries.hpp"
#include "Scene.hpp"

class SceneMain : public Based::Scene {
public:
	SceneMain (Based::Engine* engine, Based::Lua::File& conf) : Based::Scene(engine)  { }

	void handle_events (SDL_Event *event) override final { }

	void update () override final { }

	void render () override final { }

	void gui () override final { }
};
