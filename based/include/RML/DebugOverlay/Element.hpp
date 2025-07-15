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

	Rml::Element* rmlHandle;
	Type type;
	std::string rmlId;

	virtual bool needs_update (Engine& engine) const { return false; }
	inline void set_inner_rml (const std::string& rml) { rmlHandle->SetInnerRML (rml); }

	Element (Rml::ElementDocument* doc, Type type, const std::string& rmlId);

	BASED_CLASS_NO_COPY_DEFAULT_MOVE (Element);
};

/// @brief Debug overlay element which does not have to be updated (e.g. static label)
using ElementStatic = Element;

/// @brief Debug overlay element which has to be updated
struct ElementDynamic : Element {
	/// @brief Callback to get updated element text
	using UpdateCallback = const std::string& (&)(Engine& engine);
	UpdateCallback updateCb;
	virtual bool needs_update (Engine& engine) const override { return true; }

	ElementDynamic (Rml::ElementDocument* doc, Type type, const std::string& rmlId, UpdateCallback updateCb)
		: Element (doc, type, rmlId), updateCb (updateCb) { }
};

/// @brief Debug overlay element which has to be updated every tick (e.g. FPS/TPS counter)
using ElementAlwaysUpdate = ElementDynamic;

/// @brief Debug overlay element which need to update is checked by a callback (e.g. depends on external data)
struct ElementConditionalUpdate : ElementDynamic {
	/// @brief Callback to check if the element update is needed
	using NeedsUpdateCallback = bool (&)(Engine& engine);
	NeedsUpdateCallback needsUpdateCb;
	bool needs_update (Engine& engine) const override final { return needsUpdateCb (engine); }

	ElementConditionalUpdate (Rml::ElementDocument* doc, Type type, const std::string& rmlId, UpdateCallback updateCb,
	                          NeedsUpdateCallback needsUpdateCb)
		: ElementDynamic (doc, type, rmlId, updateCb), needsUpdateCb (needsUpdateCb) { }
};

}
