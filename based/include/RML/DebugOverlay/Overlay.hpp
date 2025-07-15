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

	// Use separate vectors to avoid polymorphic calls of needsUpdate()
	std::vector<ElementStatic> elems_static;
	std::vector<ElementAlwaysUpdate> elems_always_update;
	std::vector<ElementConditionalUpdate> elems_conditional_update;
	/// @brief Add a debug overlay element of a built-in type
	Element& addElement (Element::Type type);
	/// @brief Add a static custom debug overlay element
	/// @param rml_id RML id that would be used for the element
	ElementStatic& addCustomElementStatic (const std::string& rml_id);
	/// @brief Add a custom debug overlay element which has to be updated every tick
	/// @param rml_id RML id that would be used for the element
	/// @param update_cb Callback to update the element's inner RML
	ElementAlwaysUpdate& addCustomElementAlwaysUpdate (const std::string& rml_id, ElementDynamic::UpdateCallback update_cb);
	/// @brief Add a custom debug overlay element with a dynamic need to update
	/// @param rml_id RML id that would be used for the element
	/// @param update_cb Callback to update the element's inner RML
	/// @param needs_update_cb Callback to check if the update is required
	ElementConditionalUpdate& addCustomElementConditionalUpdate (const std::string& rml_id, ElementDynamic::UpdateCallback update_cb,
	                                                             ElementConditionalUpdate::NeedsUpdateCallback needs_update_cb);

	BASED_CLASS_NO_COPY_DEFAULT_MOVE (Overlay);
};

using OverlayStorage = std::unique_ptr<Overlay>;
using OverlayHandle = Overlay *;

}
