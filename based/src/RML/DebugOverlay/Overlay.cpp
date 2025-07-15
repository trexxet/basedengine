#include "RML/DebugOverlay/Overlay.hpp"

#include <format>

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

	addCustomElementStatic ("based_version").setInnerRML (std::format ("Based Engine {}", BASED_VERSION));

	rdoc->doc->Show();
}

void Overlay::update () {
	for (ElementAlwaysUpdate& elem : elems_always_update) {
		elem.setInnerRML (elem.update_cb (engine));
	}
	for (ElementConditionalUpdate& elem : elems_conditional_update) {
		if (elem.needs_update_cb (engine))
			elem.setInnerRML (elem.update_cb (engine));
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
			return elems_static.emplace_back (rdoc->doc, type, "based_label");
		case Element::Type::TPS:
			return elems_always_update.emplace_back (rdoc->doc, type, "based_tps", dou_tps);
		default: [[unlikely]]
			log.fatal ("Wrong type for debug overlay document");
			break;
	}
}
#pragma GCC diagnostic pop

ElementStatic& Overlay::addCustomElementStatic (const std::string& rml_id) {
	return elems_static.emplace_back (rdoc->doc, Element::Type::Custom, rml_id);
}

ElementAlwaysUpdate& Overlay::addCustomElementAlwaysUpdate (const std::string& rml_id, ElementDynamic::UpdateCallback update_cb) {
	return elems_always_update.emplace_back (rdoc->doc, Element::Type::Custom, rml_id, update_cb);
}

ElementConditionalUpdate& Overlay::addCustomElementConditionalUpdate (const std::string& rml_id, ElementDynamic::UpdateCallback update_cb,
                                                                      ElementConditionalUpdate::NeedsUpdateCallback needs_update_cb)
{
	return elems_conditional_update.emplace_back (rdoc->doc, Element::Type::Custom, rml_id, update_cb, needs_update_cb);
}

}
