#pragma once

#include "Scene.hpp"

#define DEMO_NAME "Demo 01-helloworld"

class SceneMain : public Based::Scene {
	Based::Size2D<int> labelSize {0, 0};
	Based::Point2D<int> labelPos {0, 0};
public:
	SceneMain (Based::Window* window, Based::Lua::File& conf) {
		window->nk_ctx()->style.window.fixed_background = nk_style_item_color (NK_COLOR_TRANSPARENT);
		labelSize = conf["labelSize"];
		labelPos.x = (window->size.width - labelSize.width) / 2;
		labelPos.y = (window->size.height - labelSize.height) / 2;
	}

	bool handle_events (SDL_Event *event) override final {
		return true;
	}

	bool update () override final {
		return true;
	}

	void render (Based::Window* window) override final {
		nk_context *nk_ctx = window->nk_ctx();
		if (nk_begin (nk_ctx, DEMO_NAME, nk_rect (labelPos.x, labelPos.y, labelSize.width, labelSize.height), NK_WINDOW_NO_SCROLLBAR)) {
			nk_layout_row_dynamic (nk_ctx, labelSize.height, 1);
			nk_label (nk_ctx, DEMO_NAME, NK_TEXT_CENTERED);
		}
		nk_end (nk_ctx);
	}
};
