#pragma once

#include "Engine.hpp"
#include "Scene.hpp"

class SceneMain : public Based::Scene {
	Based::Rect2D<int> labelRect;
public:
	SceneMain (Based::Engine *engine, Based::Lua::File& conf) : Based::Scene(engine) {
		if (!engine->client) return; // no client (headless)
		labelRect = conf["labelRect"];
	}

	// called only in client mode
	void handle_events (SDL_Event *event) override final {
		switch (event->type) {
			case SDL_EVENT_MOUSE_BUTTON_UP:
				engine->stop(); // same as engine->sceneManager.schedule_next (nullptr)
				[[fallthrough]];
			case SDL_EVENT_QUIT:
				Based::log.write ("Bye!");
				break;
			default: break;
		}
	}

	// called in both client and headless modes
	void update () override final { }

	// called only in client mode
	void render () override final { }
};
