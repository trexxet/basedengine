#pragma once

#include "NuklearAuxiliaries.hpp"
#include "Scene.hpp"

#define DEMO_NAME "Demo 01-helloworld"

class SceneMain : public Based::Scene {
	Based::Rect2D<int> labelRect;
public:
	SceneMain (Based::Window* window, Based::Lua::File& conf) {
		window->nk_ctx()->style.window.fixed_background = nk_style_item_color (NK_COLOR_TRANSPARENT);
		labelRect = conf["labelRect"];
	}

	bool handle_events (SDL_Event *event) override final {
		return true;
	}

	bool update () override final {
		return true;
	}

	void render (Based::Window* window) override final {
		nk_context *nk_ctx = window->nk_ctx();
		if (nk_begin (nk_ctx, DEMO_NAME, nk_from_Rect2D(labelRect), NK_WINDOW_NO_SCROLLBAR)) {
			nk_layout_row_dynamic (nk_ctx, labelRect.height, 1);
			nk_label (nk_ctx, DEMO_NAME, NK_TEXT_CENTERED);
		}
		nk_end (nk_ctx);
	}
};
