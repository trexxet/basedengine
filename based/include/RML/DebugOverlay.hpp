#pragma once

#include <memory>
#include <string>

#include "RML/Context.hpp"
#include "RML/Document.hpp"
#include "RML/Font.hpp"
#include "RML/Interface.hpp"

namespace Based {
	class Engine;
}

namespace Based::RML {

class DebugOverlay {
	Engine& engine;
	Interface& rml;
	ContextStorage rctx {nullptr};
	DocumentHandle rdoc {nullptr};

	bool set_font (const Font& font);
public:
	DebugOverlay (Engine& engine, Interface& rml, const Font& font);

	inline void handle_event (SDL_Event *event) { rml.handle_event (rctx, event); }
	inline void render () { rml.render (rctx); }

	static std::unique_ptr<DebugOverlay> make (Engine& engine, Interface& rml, const Font& font);
};

using DebugOverlayStorage = std::unique_ptr<DebugOverlay>;
using DebugOverlayHandle = DebugOverlay *;

}
