#pragma once

#include <SDL3/SDL_events.h>

#include "Logger.hpp"
#include "Window.hpp"

namespace Based {

class Engine;

struct Scene {
	Engine *engine = nullptr;
	Scene (Engine *engine) : engine(engine) {
		if (!engine) log.fatal ("Can't set engine for scene!");
	}
/**
 * Process input events
 * \param event pointer to SDL_Event
 * \returns true if the program keeps running
 * \note Called only in client mode
 */
	virtual bool handle_events (SDL_Event *event) = 0;
/**
 * Update internal game state
 * \returns true if the program keeps running
 * \note Called both in client and headless engine mode
 */
	virtual bool update () = 0;
/**
 * Render
 * \param window pointer to Based::Window
 * \note Called only in client mode
 */
	virtual void render () = 0;
/**
 * Process Nuklear GUI
 * \param window pointer to Based::Window
 * \returns true if the program keeps running
 * \note Called only in client mode
 */
	virtual bool gui () = 0;
};

class SceneManager {
	friend class Engine;
	Scene *currentScene = nullptr;
	Scene *scheduledScene = nullptr;
	inline void commit_scheduled () {
		if (scheduledScene != currentScene) [[unlikely]]
			currentScene = scheduledScene;
	}
public:
/**
 * Set the scene for the next tick
 * \param nextScene pointer to the next scene or null if the tick is last
 */
	inline void schedule_next (Scene *nextScene) {
		scheduledScene = nextScene;
	}

	bool handle_events (SDL_Event *event);
	bool update ();
	void render ();
	bool gui ();
};

}
