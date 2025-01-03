#include "Scene.hpp"

namespace Based {

void SceneManager::handle_events () {
	if (currentScene)
		currentScene->handle_events();
}

void SceneManager::update () {
	if (currentScene)
		currentScene->update();
}

void SceneManager::render () {
	if (currentScene)
		currentScene->render();
}

}
