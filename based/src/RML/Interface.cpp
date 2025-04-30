#include "RML/Interface.hpp"

#include <RmlUi/Core/Core.h>
#include <RmlUi/Debugger.h>

#include "Logger.hpp"
#include "Window.hpp"

namespace Based::RML {

ContextStorage Interface::make_context (const std::string& name) {
	return Context::make (name, owner->size);
}

void Interface::load_fonts (const std::vector<std::string>& paths) {
	for (const std::string& path: paths) {
		if (!Rml::LoadFontFace(path))
			log.fatal ("Failed to load font {}", path);
	}
}

void Interface::init_debugger (ContextStorage& rctx, SDL_Keycode toggleKey) {
	Rml::Debugger::Initialise(rctx->rctx);
	debuggerToggleKey = toggleKey;
}

void Interface::handle_event (ContextStorage& rctx, SDL_Event *event) {
	if (event->type == SDL_EVENT_KEY_DOWN) {
		if (event->key.key == debuggerToggleKey)
			Rml::Debugger::SetVisible (!Rml::Debugger::IsVisible());
	}
	RmlSDL::InputEventHandler (rctx->rctx, owner->sdlWindow, *event);
}

void Interface::render (ContextStorage& rctx) {
	rctx->rctx->Update();
	// TODO: render_interface.Clear() - is it needed?
	renderInterface.BeginFrame();
	rctx->rctx->Render();
	renderInterface.EndFrame();
}

Interface::Interface (Window* owner) : owner (owner) {
	if (!owner->sdlWindow)
		log.fatal ("Failed to initialize RML: window is not created yet");
	if (!renderInterface)
		log.fatal ("Failed to initialize RML GL3 interface");

	Rml::SetRenderInterface (&renderInterface);
	Rml::SetSystemInterface (&systemInterface);

	systemInterface.SetWindow (owner->sdlWindow);
	renderInterface.SetViewport (owner->size.x, owner->size.y);

	if (!Rml::Initialise())
		log.fatal ("Failed to initialize RML");
	log.write ("RML {} initialized", Rml::GetVersion());
}

Interface::~Interface () {
	Rml::Shutdown();
}

}
