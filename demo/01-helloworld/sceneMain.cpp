#include "sceneMain.hpp"

#include "Logger.hpp"

bool SceneMain::handle_events (SDL_Event *event) {
	return true;
}

bool SceneMain::update () {
	Based::log.write ("Scene main update");
	return true;
}

void SceneMain::render () {
	Based::log.write ("Scene main render");
}
