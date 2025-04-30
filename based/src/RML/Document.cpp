#include "RML/Document.hpp"

#include "Logger.hpp"

namespace Based::RML {

Document::Document (Rml::Context* rctx, bool fromString) : rctx (rctx), fromString (fromString) {
	if (!rctx)
		log.fatal ("Failed to create RML document: null context");
}

void Document::load (const std::string& source) {
	if (fromString)
		doc = rctx->LoadDocumentFromMemory (source);
	else
		doc = rctx->LoadDocument (source);
	if (!doc)
		log.fatal ("Failed to load RML document from {}",
		           fromString ? "string" : source);
}

std::unique_ptr<Document> Document::make_from_file (Rml::Context* rctx, const std::string& path) {
	std::unique_ptr<Document> rdoc = std::make_unique<Document> (rctx, false);
	rdoc->load (path);
	return rdoc;
}

std::unique_ptr<Document> Document::make_from_string (Rml::Context* rctx, const std::string& src) {
	std::unique_ptr<Document> rdoc = std::make_unique<Document> (rctx, true);
	rdoc->load (src);
	return rdoc;
}

}
