#include "Scene.hpp"

namespace Based {

void SceneManager::update () {
	if (currentScene)
		currentScene->update();
}

void SceneManager::render () {
	if (currentScene)
		currentScene->render();
}

void SceneManager::handleEvents () {
	if (currentScene)
		currentScene->handleEvents();
}

}
