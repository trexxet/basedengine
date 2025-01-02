#pragma once

#include "Scene.hpp"

class SceneMain : public Based::Scene {
public:
	void update () override final;
	void render () override final;
	void handleEvents () override final;
};
