#pragma once

#include <SDL3/SDL_events.h>

#include "Logger.hpp"
#include "Window.hpp"

namespace Based {

class Engine;
class EngineClient;

struct Scene {
	Engine& engine;

	Scene (Engine& engine) : engine(engine) { }

	/**
	 * Process input events
	 * \param event pointer to SDL_Event
	 * \note Called only in client mode
	 */
	virtual void handle_events (SDL_Event *event) = 0;

	/**
	 * Update internal game state
	 * \note Called both in client and headless engine mode
	 */
	virtual void update () = 0;

	/**
	 * Render
	 * \note Called only in client mode
	 */
	virtual void render () = 0;
};

class SceneManager {
	friend class Engine;
	friend class EngineClient;

	// Scene switch scheduling
	Scene *currentScene = nullptr;
	Scene *scheduledScene = nullptr;
	inline bool commit_scheduled () {
		if (scheduledScene != currentScene) [[unlikely]]
			currentScene = scheduledScene;
		return (currentScene != nullptr);
	}

	// Tick pipeline
	inline void handle_events (SDL_Event *event) {
		if (currentScene) [[likely]]
			currentScene->handle_events (event);
	}
	inline void update () {
		if (currentScene) [[likely]]
			currentScene->update();
	}
	inline void render () {
		if (currentScene) [[likely]]
			currentScene->render();
	}

public:
/**
 * Set the scene for the next tick
 * \param nextScene pointer to the next scene or null if the tick is last
 */
	inline void schedule_next (Scene *nextScene) {
		scheduledScene = nextScene;
	}
};

}
