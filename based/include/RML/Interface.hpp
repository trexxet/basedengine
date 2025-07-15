#pragma once

#include <memory>
#include <string>
#include <vector>

#include <RmlUi_Renderer_GL3.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>

#include "RML/Context.hpp"
#include "RML/Document.hpp"
#include "RML/Model.hpp"
#include "RML/SystemInterface_SDL_Based.hpp"

namespace Based {
	class Window;
}

namespace Based::RML {

class Interface {
	SystemInterface_SDL_Based systemInterface;
	RenderInterface_GL3 renderInterface;
	Window& owner;
public:
	/**
	 * \brief Create Rml context
	 * \param name unique context name
	 * \returns Context pointer if created successfully, otherwise throws an exception
	 */
	ContextStorage make_context (const std::string& name);

	static void load_fonts (const std::vector<std::string>& paths);

	SDL_Keycode rmlDebuggerToggleKey = SDLK_UNKNOWN;
	void init_rml_debugger (ContextStorage& rctx, SDL_Keycode toggleKey = SDLK_F12);

	void handle_event (ContextStorage& rctx, SDL_Event *event);
	void render (ContextStorage& rctx);

	Interface (Window& owner);
	~Interface ();
};

using InterfaceStorage = std::unique_ptr<Interface>;
using InterfaceHandle = Interface *;

}
