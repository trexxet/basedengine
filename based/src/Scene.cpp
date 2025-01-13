#include "Scene.hpp"

namespace Based {

bool SceneManager::handle_events (SDL_Event *event) {
	if (currentScene) [[likely]]
		return currentScene->handle_events (event);
	return (scheduledScene != nullptr);
}

bool SceneManager::update () {
	if (currentScene) [[likely]]
		return currentScene->update();
	return (scheduledScene != nullptr);
}

void SceneManager::render () {
	if (currentScene) [[likely]]
		currentScene->render();
}

bool SceneManager::gui () {
	if (currentScene) [[likely]]
		return currentScene->gui();
	return (scheduledScene != nullptr);
}

}
