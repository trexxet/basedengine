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

void SceneManager::render (Window* window) {
	if (currentScene) [[likely]]
		currentScene->render(window);
}

bool SceneManager::gui (Window* window) {
	if (currentScene) [[likely]]
		return currentScene->gui(window);
	return (scheduledScene != nullptr);
}

}
