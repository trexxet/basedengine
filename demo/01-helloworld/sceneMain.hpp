#pragma once

#include "Scene.hpp"

class SceneMain : public Based::Scene {
public:
	void handle_events () override final;
	void update () override final;
	void render () override final;
};
