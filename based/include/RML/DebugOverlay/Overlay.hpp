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

class BASED_API Overlay {
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
	std::vector<ElementStatic> elemsStatic;
	std::vector<ElementAlwaysUpdate> elemsAlwaysUpdate;
	std::vector<ElementConditionalUpdate> elemsConditionalUpdate;
	/// @brief Add a debug overlay element of a built-in type
	Element& add_element (Element::Type type);
	/// @brief Add a static custom debug overlay element
	/// @param rmlId RML id that would be used for the element
	ElementStatic& add_custom_element_static (const std::string& rmlId);
	/// @brief Add a custom debug overlay element which has to be updated every tick
	/// @param rmlId RML id that would be used for the element
	/// @param updateCb Callback to update the element's inner RML
	ElementAlwaysUpdate& add_custom_element_always_update (const std::string& rmlId, ElementDynamic::UpdateCallback updateCb);
	/// @brief Add a custom debug overlay element with a dynamic need to update
	/// @param rmlId RML id that would be used for the element
	/// @param updateCb Callback to update the element's inner RML
	/// @param needsUpdateCb Callback to check if the update is required
	ElementConditionalUpdate& add_custom_element_conditional_update (const std::string& rmlId, ElementDynamic::UpdateCallback updateCb,
	                                                                 ElementConditionalUpdate::NeedsUpdateCallback needsUpdateCb);

	BASED_CLASS_NO_COPY_DEFAULT_MOVE (Overlay);
};

using OverlayStorage = std::unique_ptr<Overlay>;
using OverlayHandle = Overlay *;

}
