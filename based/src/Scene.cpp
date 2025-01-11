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

void SceneManager::render (Window* window) {
	if (currentScene)
		currentScene->render(window);
}

bool SceneManager::gui (Window* window) {
	if (currentScene)
		return currentScene->gui(window);
	return false;
}

}
