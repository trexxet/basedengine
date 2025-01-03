#include "sceneMain.hpp"

#include "Logger.hpp"

void SceneMain::handle_events () {
	Based::log.write ("Scene main events");
}

void SceneMain::update () {
	Based::log.write ("Scene main update");
}

void SceneMain::render () {
	Based::log.write ("Scene main render");
}
