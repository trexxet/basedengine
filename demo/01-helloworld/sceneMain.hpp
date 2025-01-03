#pragma once

#include "Scene.hpp"

class SceneMain : public Based::Scene {
public:
	bool handle_events (SDL_Event *event) override final;
	bool update () override final;
	void render () override final;
};
