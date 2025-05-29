#include "RML/DebugOverlay.hpp"

#include <RmlUi/Core/ElementDocument.h>

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

std::unique_ptr<DebugOverlay> DebugOverlay::make (Engine& engine, Interface& rml, const Font& font) {
	return std::make_unique<DebugOverlay> (engine, rml, font);
}

}
