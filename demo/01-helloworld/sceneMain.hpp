#pragma once

#include "Engine.hpp"
#include "NuklearAuxiliaries.hpp"
#include "Scene.hpp"

class SceneMain : public Based::Scene {
	Based::Rect2D<int> labelRect;
public:
	SceneMain (Based::Engine *engine, Based::Lua::File& conf) : Based::Scene(engine) {
		if (!engine->client) return; // no client (headless)
		nk_context *nk_ctx = engine->client->window()->nk_ctx();
		nk_ctx->style.window.fixed_background = nk_style_item_color (NK_COLOR_TRANSPARENT);
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

	// called only in client mode
	void gui () override final {
		nk_context *nk_ctx = engine->client->window()->nk_ctx();
		if (nk_begin (nk_ctx, DEMO_NAME, nk_from_Rect2D(labelRect), NK_WINDOW_NO_SCROLLBAR)) {
			nk_layout_row_dynamic (nk_ctx, labelRect.height, 1);
			nk_label (nk_ctx, DEMO_NAME, NK_TEXT_CENTERED);
		}
		nk_end (nk_ctx);
	}
};
