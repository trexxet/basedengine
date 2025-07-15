#include "RML/DebugOverlay/Overlay.hpp"

#include <RmlUi/Core/ElementDocument.h>

#include "Logger.hpp"

#include "RML/DebugOverlay/Callbacks.hpp"
#include "RML/DebugOverlay/RmlDoc.hpp"

namespace Based::RML::DebugOverlay {

bool Overlay::set_font (const Font& font) {
	Rml::ElementDocument* doc = rdoc->doc;

	return doc->SetProperty (Rml::PropertyId::FontFamily, Rml::Property(font.family, Rml::Unit::STRING)) &&
	       doc->SetProperty (Rml::PropertyId::FontStyle, Rml::Property(font.style)) &&
	       doc->SetProperty (Rml::PropertyId::FontWeight, Rml::Property(font.weight)) &&
	       doc->SetProperty (Rml::PropertyId::FontSize, Rml::Property(font.size_px, Rml::Unit::PX)) &&
	       doc->SetProperty (Rml::PropertyId::Color, Rml::Property(font.color, Rml::Unit::STRING));
}

Overlay::Overlay (Engine& engine, Interface& rml, const Font& font) : engine(engine), rml(rml) {
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

void Overlay::update () {
	for (const ElementFixedUpdate& elem : elems_fixed_update) {
		if (elem.needs_update_fixed)
			elem.rml_handle->SetInnerRML (elem.update_cb (engine));
	}
	for (const ElementDynamicUpdate& elem : elems_dynamic_update) {
		if (elem.needs_update_cb (engine))
			elem.rml_handle->SetInnerRML (elem.update_cb (engine));
	}
}

std::unique_ptr<Overlay> Overlay::make (Engine& engine, Interface& rml, const Font& font) {
	return std::make_unique<Overlay> (engine, rml, font);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
Element& Overlay::addElement (Element::Type type) {
	switch (type) {
		case Element::Type::Custom: [[unlikely]]
			log.fatal ("Use addCustomElement*() to create a custom debug overlay element");
			break;
		case Element::Type::Label:
			return elems_fixed_update.emplace_back (rdoc->doc, type, "based_label", "", ElementFixedUpdate::update_cb_never_stub, false);
		case Element::Type::TPS:
			return elems_fixed_update.emplace_back (rdoc->doc, type, "based_fps", "0", dou_tps, true);
		default: [[unlikely]]
			log.fatal ("Wrong type for debug overlay document");
			break;
	}
}
#pragma GCC diagnostic pop

ElementFixedUpdate& Overlay::addCustomElementFixedUpdate (const std::string& rml_id, const std::string& inner_rml,
                                                          Element::UpdateCallback update_cb, bool needs_update_fixed)
{
	return elems_fixed_update.emplace_back (rdoc->doc, Element::Type::Custom, rml_id, inner_rml, update_cb, needs_update_fixed);
}

ElementDynamicUpdate& Overlay::addCustomElementDynamicUpdate (const std::string& rml_id, const std::string& inner_rml,
                                                              Element::UpdateCallback update_cb, ElementDynamicUpdate::NeedsUpdateCallback needs_update_cb)
{
	return elems_dynamic_update.emplace_back (rdoc->doc, Element::Type::Custom, rml_id, inner_rml, update_cb, needs_update_cb);
}

}
