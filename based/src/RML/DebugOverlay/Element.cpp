#include "RML/DebugOverlay/Element.hpp"

#include <format>

#include "Engine.hpp"

#include "RML/DebugOverlay/Callbacks.hpp"

namespace Based::RML::DebugOverlay {

Element::Element (Rml::ElementDocument* doc, Type type, const std::string& rml_id, const std::string& inner_rml, UpdateCallback update_cb)
	: type (type), rml_id (rml_id), update_cb (update_cb)
{
	rml_handle = doc->AppendChild (doc->CreateElement ("p"));
	rml_handle->SetId (rml_id);
	rml_handle->SetInnerRML (inner_rml);
}

/// @brief Update callback stub for never updated elements like static labels. Shouldn't get called.
static const std::string Element_UpdateCbStub_str {"based"};
BASED_DEBUG_OVERLAY_UPDATE_CB(ElementFixedUpdate::update_cb_never_stub) { return Element_UpdateCbStub_str; }

/// @brief Update callback for TPS counter
BASED_DEBUG_OVERLAY_UPDATE_CB(dou_tps) {
	thread_local static std::string tps;
	tps = std::format ("TPS: {}", engine.tps.get());
	return tps;
}

}
