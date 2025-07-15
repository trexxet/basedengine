#pragma once

#include <string>

#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>

#include "Defs.hpp"

namespace Based {
	class Engine;
}

namespace Based::RML::DebugOverlay {

struct Element {
	enum class Type {
		Custom,
		Label,
		TPS
	};
	/// @brief Callback to get updated element text
	using UpdateCallback = const std::string& (&)(Engine& engine);

	Rml::Element* rml_handle;
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

	/// @brief Update callback stub for never updated elements like static labels. Shouldn't get called.
	static const std::string& update_cb_never_stub (Engine& engine);
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

}
