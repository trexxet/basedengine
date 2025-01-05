#pragma once

#include <SDL3/SDL_events.h>

#include "Window.hpp"

namespace Based {

class Scene {
public:
/**
 * Process input events
 *
 * \param event pointer to SDL_Event
 * \returns true if the program keeps runnign
 */
	virtual bool handle_events (SDL_Event *event) = 0;
/**
 * Update internal game state
 *
 * \returns true if the program keeps runnign
 */
	virtual bool update () = 0;
/**
 * Render. Also, process Nuklear GUI.
 *
 * \param window pointer to Based::Window
 */
	virtual void render (Window* window) = 0;
};

struct SceneManager {
	Scene *currentScene = nullptr;
	bool handle_events (SDL_Event *event);
	bool update ();
	void render (Window* window);
};

}
