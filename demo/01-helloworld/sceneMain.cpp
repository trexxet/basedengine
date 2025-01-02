#include "sceneMain.hpp"

#include "Logger.hpp"

void SceneMain::update () {
	Based::log.write ("Scene main update");
}

void SceneMain::render () {
	Based::log.write ("Scene main render");
}

void SceneMain::handleEvents () {
	Based::log.write ("Scene main events");
}
