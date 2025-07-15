#pragma once

#include <string>

#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/ElementDocument.h>

#include "Defs.hpp"

namespace Based {
	class Engine;
}

namespace Based::RML::DebugOverlay {

/// @brief Base element class, basically a static element
struct Element {
	enum class Type {
		Custom,
		Label,
		TPS
	};

	Rml::Element* rml_handle;
	Type type;
	std::string rml_id;

	virtual bool needsUpdate (Engine& engine) const { return false; }
	inline void setInnerRML (const std::string& rml) { rml_handle->SetInnerRML (rml); }

	Element (Rml::ElementDocument* doc, Type type, const std::string& rml_id);

	BASED_CLASS_NO_COPY_DEFAULT_MOVE (Element);
};

/// @brief Debug overlay element which does not have to be updated (e.g. static label)
using ElementStatic = Element;

/// @brief Debug overlay element which has to be updated
struct ElementDynamic : Element {
	/// @brief Callback to get updated element text
	using UpdateCallback = const std::string& (&)(Engine& engine);
	UpdateCallback update_cb;
	virtual bool needsUpdate (Engine& engine) const override { return true; }

	ElementDynamic (Rml::ElementDocument* doc, Type type, const std::string& rml_id, UpdateCallback update_cb)
		: Element (doc, type, rml_id), update_cb (update_cb) { }
};

/// @brief Debug overlay element which has to be updated every tick (e.g. FPS/TPS counter)
using ElementAlwaysUpdate = ElementDynamic;

/// @brief Debug overlay element which need to update is checked by a callback (e.g. depends on external data)
struct ElementConditionalUpdate : ElementDynamic {
	/// @brief Callback to check if the element update is needed
	using NeedsUpdateCallback = bool (&)(Engine& engine);
	NeedsUpdateCallback needs_update_cb;
	bool needsUpdate (Engine& engine) const override final { return needs_update_cb (engine); }

	ElementConditionalUpdate (Rml::ElementDocument* doc, Type type, const std::string& rml_id, UpdateCallback update_cb,
	                          NeedsUpdateCallback needs_update_cb)
		: ElementDynamic (doc, type, rml_id, update_cb), needs_update_cb (needs_update_cb) { }
};

}
