#pragma once

#include "Engine.hpp"
#include "NuklearAuxiliaries.hpp"
#include "Scene.hpp"

class SceneMain : public Based::Scene {
public:
	SceneMain (Based::Engine *engine, Based::Lua::File& conf) : Based::Scene(engine) {
		if (!engine->client) return;
	}

	void handle_events (SDL_Event *event) override final {
		switch (event->type) {
			case SDL_EVENT_MOUSE_BUTTON_UP:
				engine->stop();
				[[fallthrough]];
			case SDL_EVENT_QUIT:
				Based::log.write ("Bye!");
				break;
			default: break;
		}
	}

	void update () override final { }

	void render () override final { }

	void gui () override final { }
};
