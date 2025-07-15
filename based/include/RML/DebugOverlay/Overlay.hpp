#pragma once

#include <memory>
#include <string>

#include "Defs.hpp"
#include "RML/Context.hpp"
#include "RML/Document.hpp"
#include "RML/Font.hpp"
#include "RML/Interface.hpp"
#include "RML/DebugOverlay/Element.hpp"

namespace Based {
	class Engine;
}

namespace Based::RML::DebugOverlay {

class Overlay {
	Engine& engine;
	Interface& rml;
	ContextStorage rctx {nullptr};
	DocumentHandle rdoc {nullptr};

	bool set_font (const Font& font);
public:
	Overlay (Engine& engine, Interface& rml, const Font& font);

	void update ();
	inline void handle_event (SDL_Event *event) { rml.handle_event (rctx, event); }
	inline void render () { rml.render (rctx); }

	static std::unique_ptr<Overlay> make (Engine& engine, Interface& rml, const Font& font);

	// Use separate vectors to avoid polymorphic calls of needs_update()
	std::vector<ElementFixedUpdate> elems_fixed_update;
	std::vector<ElementDynamicUpdate> elems_dynamic_update;
	/// @brief Add a debug overlay element of a built-in type
	Element& addElement (Element::Type type);
	/// @brief Add a custom debug overlay element with a fixed need to update
	/// @param rml_id RML id that would be used for the element
	/// @param inner_rml Initial inner RML of created element
	/// @param needs_update_fixed If the element needs to be updated by update_cb callback
	ElementFixedUpdate& addCustomElementFixedUpdate (const std::string& rml_id, const std::string& inner_rml,
	                                                 Element::UpdateCallback update_cb, bool needs_update_fixed);
	/// @brief Add a custom debug overlay element with a dynamic need to update
	/// @param rml_id RML id that would be used for the element
	/// @param inner_rml Initial inner RML of created element
	/// @param needs_update_fixed If the element needs to be updated by update_cb callback
	ElementDynamicUpdate& addCustomElementDynamicUpdate (const std::string& rml_id, const std::string& inner_rml,
	                                                     Element::UpdateCallback update_cb, ElementDynamicUpdate::NeedsUpdateCallback needs_update_cb);

	BASED_CLASS_NO_COPY_DEFAULT_MOVE (Overlay);
};

using OverlayStorage = std::unique_ptr<Overlay>;
using OverlayHandle = Overlay *;

}
