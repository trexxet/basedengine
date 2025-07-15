#include "RML/DebugOverlay/Element.hpp"

#include <format>

#include "Logger.hpp"
#include "Engine.hpp"

#include "RML/DebugOverlay/Callbacks.hpp"

namespace Based::RML::DebugOverlay {

Element::Element (Rml::ElementDocument* doc, Type type, const std::string& rmlId) : type (type), rmlId (rmlId) {
	rmlHandle = doc->AppendChild (doc->CreateElement ("p"));
	if (!rmlHandle)
		log.fatal ("Failed to add debug overlay element {}", rmlId);
	rmlHandle->SetId (rmlId);
}

/// @brief Update callback for TPS counter
BASED_DEBUG_OVERLAY_UPDATE_CB(dou_tps) {
	thread_local static std::string tps;
	tps = std::format ("TPS: {}", engine.tps.get());
	return tps;
}

}
