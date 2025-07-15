#include "RML/DebugOverlay/Element.hpp"

#include <format>

#include "Logger.hpp"
#include "Engine.hpp"

#include "RML/DebugOverlay/Callbacks.hpp"

namespace Based::RML::DebugOverlay {

Element::Element (Rml::ElementDocument* doc, Type type, const std::string& rml_id) : type (type), rml_id (rml_id) {
	rml_handle = doc->AppendChild (doc->CreateElement ("p"));
	if (!rml_handle)
		log.fatal ("Failed to add debug overlay element {}", rml_id);
	rml_handle->SetId (rml_id);
}

/// @brief Update callback for TPS counter
BASED_DEBUG_OVERLAY_UPDATE_CB(dou_tps) {
	thread_local static std::string tps;
	tps = std::format ("TPS: {}", engine.tps.get());
	return tps;
}

}
