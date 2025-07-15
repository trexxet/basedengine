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

	add_custom_element_static ("based_version").set_inner_rml (std::format ("Based Engine {}", BASED_VERSION));

	rdoc->doc->Show();
}

void Overlay::update () {
	for (ElementAlwaysUpdate& elem : elemsAlwaysUpdate) {
		elem.set_inner_rml (elem.updateCb (engine));
	}
	for (ElementConditionalUpdate& elem : elemsConditionalUpdate) {
		if (elem.needsUpdateCb (engine))
			elem.set_inner_rml (elem.updateCb (engine));
	}
}

std::unique_ptr<Overlay> Overlay::make (Engine& engine, Interface& rml, const Font& font) {
	return std::make_unique<Overlay> (engine, rml, font);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
Element& Overlay::add_element (Element::Type type) {
	switch (type) {
		case Element::Type::Custom: [[unlikely]]
			log.fatal ("Use addCustomElement*() to create a custom debug overlay element");
			break;
		case Element::Type::Label:
			return elemsStatic.emplace_back (rdoc->doc, type, "based_label");
		case Element::Type::TPS:
			return elemsAlwaysUpdate.emplace_back (rdoc->doc, type, "based_tps", dou_tps);
		default: [[unlikely]]
			log.fatal ("Wrong type for debug overlay document");
			break;
	}
}
#pragma GCC diagnostic pop

ElementStatic& Overlay::add_custom_element_static (const std::string& rmlId) {
	return elemsStatic.emplace_back (rdoc->doc, Element::Type::Custom, rmlId);
}

ElementAlwaysUpdate& Overlay::add_custom_element_always_update (const std::string& rmlId, ElementDynamic::UpdateCallback updateCb) {
	return elemsAlwaysUpdate.emplace_back (rdoc->doc, Element::Type::Custom, rmlId, updateCb);
}

ElementConditionalUpdate& Overlay::add_custom_element_conditional_update (const std::string& rmlId, ElementDynamic::UpdateCallback updateCb,
                                                                          ElementConditionalUpdate::NeedsUpdateCallback needsUpdateCb)
{
	return elemsConditionalUpdate.emplace_back (rdoc->doc, Element::Type::Custom, rmlId, updateCb, needsUpdateCb);
}

}
