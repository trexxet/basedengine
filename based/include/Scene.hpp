#pragma once

#include <SDL3/SDL_events.h>

namespace Based {

class Scene {
public:
	virtual bool handle_events (SDL_Event *event) = 0;
	virtual bool update () = 0;
	virtual void render () = 0;
};

struct SceneManager {
	Scene *currentScene = nullptr;
	bool handle_events (SDL_Event *event);
	bool update ();
	void render ();
};

}
