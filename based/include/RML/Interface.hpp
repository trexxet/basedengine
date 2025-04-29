#pragma once

#include <string>
#include <vector>

#include <RmlUi_Renderer_GL3.h>
#include <RmlUi_Platform_SDL.h>
#include <SDL3/SDL_events.h>

#include "RML/Context.hpp"
#include "RML/Document.hpp"
#include "RML/Model.hpp"

namespace Based {
	class Window;
}
using Based::Window;

namespace Based::RML {

struct Interface {
	SystemInterface_SDL systemInterface;
	RenderInterface_GL3 renderInterface;
	Window* owner;

	/**
	 * \brief Create Rml context
	 * \param name unique context name
	 * \returns Context pointer if created successfully, otherwise throws an exception
	 */
	ContextStorage make_context (const std::string& name);

	static void load_fonts (const std::vector<std::string>& paths);

	SDL_Keycode debuggerToggleKey = SDLK_UNKNOWN;
	void init_debugger (ContextStorage& rctx, SDL_Keycode toggleKey = SDLK_F12);

	void handle_event (ContextStorage& rctx, SDL_Event *event);
	void render (ContextStorage& rctx);

	Interface (Window* owner);
	~Interface ();
};

using InterfaceHandle = Interface *;

}
