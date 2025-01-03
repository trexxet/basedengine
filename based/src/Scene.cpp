#include "Scene.hpp"

namespace Based {

bool SceneManager::handle_events (SDL_Event *event) {
	if (currentScene)
		return currentScene->handle_events (event);
	return false;
}

bool SceneManager::update () {
	if (currentScene)
		return currentScene->update();
	return false;
}

void SceneManager::render () {
	if (currentScene)
		currentScene->render();
}

}
