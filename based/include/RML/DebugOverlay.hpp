#pragma once

#include <memory>
#include <string>

#include <RmlUi/Core/ElementDocument.h>

#include "Defs.hpp"
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

	void update ();
	inline void handle_event (SDL_Event *event) { rml.handle_event (rctx, event); }
	inline void render () { rml.render (rctx); }

	static std::unique_ptr<DebugOverlay> make (Engine& engine, Interface& rml, const Font& font);

	struct Element {
		using RmlElementHandle = Rml::Element*;
		using RmlElementDocumentHandle = Rml::ElementDocument*;
		enum class Type {
			Custom,
			Label,
			TPS
		};
		/// @brief Callback to get updated element text
		using UpdateCallback = const std::string& (&)(Engine& engine);

		RmlElementHandle rml_handle;
		Type type;
		std::string rml_id;

		virtual bool needs_update (Engine& engine) const = 0;
		UpdateCallback update_cb;

		Element (Rml::ElementDocument* doc, Type type, const std::string& rml_id, const std::string& inner_rml, UpdateCallback update_cb);

		BASED_CLASS_NO_COPY_DEFAULT_MOVE (Element);
	};

	/// @brief Debug overlay element, which has a fixed need to update (e.g. FPS counter, static label etc.)
	struct ElementFixedUpdate : Element {
		bool needs_update_fixed;
		bool needs_update (Engine& engine) const override final { return needs_update_fixed; }

		ElementFixedUpdate (Rml::ElementDocument* doc, Type type, const std::string& rml_id, const std::string& inner_rml,
		                    UpdateCallback update_cb, bool needs_update_fixed)
			: Element (doc, type, rml_id, inner_rml, update_cb), needs_update_fixed (needs_update_fixed) { }
	};

	/// @brief Debug overlay element, which need to update is checked by a callback (e.g. depends on external data)
	struct ElementDynamicUpdate : Element {
		/// @brief Callback to check if the element update is needed
		using NeedsUpdateCallback = bool (&)(Engine& engine);
		NeedsUpdateCallback needs_update_cb;
		bool needs_update (Engine& engine) const override final { return needs_update_cb(engine); }

		ElementDynamicUpdate (Rml::ElementDocument* doc, Type type, const std::string& rml_id, const std::string& inner_rml,
		                      UpdateCallback update_cb, NeedsUpdateCallback needs_update_cb)
			: Element (doc, type, rml_id, inner_rml, update_cb), needs_update_cb (needs_update_cb) { }
	};

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

	BASED_CLASS_NO_COPY_DEFAULT_MOVE (DebugOverlay);
};

/// @brief Update callback stub for never updated elements like static labels. Shouldn't get called.
const std::string& DebugOverlayElement_UpdateCbStub (Engine& engine);

using DebugOverlayStorage = std::unique_ptr<DebugOverlay>;
using DebugOverlayHandle = DebugOverlay *;

}
