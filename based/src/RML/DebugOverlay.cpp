#include "RML/DebugOverlay.hpp"

#include "Engine.hpp"
#include "Logger.hpp"

#include "RML/DebugOverlayData.hpp"

namespace Based::RML {

bool DebugOverlay::set_font (const Font& font) {
	Rml::ElementDocument* doc = rdoc->doc;

	return doc->SetProperty (Rml::PropertyId::FontFamily, Rml::Property(font.family, Rml::Unit::STRING)) &&
	       doc->SetProperty (Rml::PropertyId::FontStyle, Rml::Property(font.style)) &&
	       doc->SetProperty (Rml::PropertyId::FontWeight, Rml::Property(font.weight)) &&
	       doc->SetProperty (Rml::PropertyId::FontSize, Rml::Property(font.size_px, Rml::Unit::PX)) &&
	       doc->SetProperty (Rml::PropertyId::Color, Rml::Property(font.color, Rml::Unit::STRING));
}

DebugOverlay::DebugOverlay (Engine& engine, Interface& rml, const Font& font) : engine(engine), rml(rml) {
	rctx = rml.make_context ("__basedCtxDebug");

	rdoc = rctx->add_document_from_string ("__basedDocDebug", debugOverlayRmlDoc);
	if (!rdoc)
		log.fatal ("Failed to load debug overlay RML document!");

	if (!set_font (font))
		log.fatal ("Can't set debug overlay font!");

	Rml::ElementDocument* doc = rdoc->doc;
	doc->GetElementById ("based_version")->SetInnerRML (std::string ("Based Engine ") + BASED_VERSION);

	doc->Show();
}

void DebugOverlay::update () {
	for (const DebugOverlay::ElementFixedUpdate& elem : elems_fixed_update) {
		if (elem.needs_update_fixed)
			elem.rml_handle->SetInnerRML (elem.update_cb (engine));
	}
	for (const DebugOverlay::ElementDynamicUpdate& elem : elems_dynamic_update) {
		if (elem.needs_update_cb (engine))
			elem.rml_handle->SetInnerRML (elem.update_cb (engine));
	}
}

std::unique_ptr<DebugOverlay> DebugOverlay::make (Engine& engine, Interface& rml, const Font& font) {
	return std::make_unique<DebugOverlay> (engine, rml, font);
}

#define DEBUG_OVERLAY_UPDATE_CB(fn) const std::string& fn (Engine& engine)

DEBUG_OVERLAY_UPDATE_CB(dou_tps) {
	thread_local static std::string tps;
	tps = std::format ("TPS: {}", engine.tps.get());
	return tps;
}

#undef DEBUG_OVERLAY_UPDATE_CB

DebugOverlay::Element::Element (Rml::ElementDocument* doc, DebugOverlay::Element::Type type, const std::string& rml_id,
                                const std::string& inner_rml, UpdateCallback update_cb) : type (type), rml_id (rml_id), update_cb (update_cb)
{
	rml_handle = doc->AppendChild (doc->CreateElement ("p"));
	rml_handle->SetId (rml_id);
	rml_handle->SetInnerRML (inner_rml);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
DebugOverlay::Element& DebugOverlay::addElement (DebugOverlay::Element::Type type) {
	switch (type) {
		case DebugOverlay::Element::Type::Custom: [[unlikely]]
			log.fatal ("Use addCustomElement*() to create a custom debug overlay element");
			break;
		case DebugOverlay::Element::Type::Label:
			return elems_fixed_update.emplace_back (rdoc->doc, type, "based_label", "", DebugOverlayElement_UpdateCbStub, false);
		case DebugOverlay::Element::Type::TPS:
			return elems_fixed_update.emplace_back (rdoc->doc, type, "based_fps", "0", dou_tps, true);
		default: [[unlikely]]
			log.fatal ("Wrong type for debug overlay document");
			break;
	}
}
#pragma GCC diagnostic pop

DebugOverlay::ElementFixedUpdate& DebugOverlay::addCustomElementFixedUpdate (const std::string& rml_id, const std::string& inner_rml,
                                                                             Element::UpdateCallback update_cb, bool needs_update_fixed)
{
	return elems_fixed_update.emplace_back (rdoc->doc, DebugOverlay::Element::Type::Custom, rml_id, inner_rml, update_cb, needs_update_fixed);
}

DebugOverlay::ElementDynamicUpdate& DebugOverlay::addCustomElementDynamicUpdate (const std::string& rml_id, const std::string& inner_rml,
                                                                                 Element::UpdateCallback update_cb, ElementDynamicUpdate::NeedsUpdateCallback needs_update_cb)
{
	return elems_dynamic_update.emplace_back (rdoc->doc, DebugOverlay::Element::Type::Custom, rml_id, inner_rml, update_cb, needs_update_cb);
}

/// @brief Update callback stub for never updated elements like static labels. Shouldn't get called.
const std::string DebugOverlayElement_UpdateCbStub_str {"based"};
const std::string& DebugOverlayElement_UpdateCbStub (Engine& engine) { return DebugOverlayElement_UpdateCbStub_str; }

}
