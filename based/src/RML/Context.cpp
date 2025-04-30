#include "RML/Context.hpp"

#include <RmlUi/Core/Core.h>

#include "Logger.hpp"
#include "RML/RML_Util.hpp"

namespace Based::RML {

Context::Context (const std::string& ctxName, const Vec2D<int>& dimensions) : ctxName (ctxName) {
	_rctx = Rml::CreateContext (ctxName, to_Vec2 (dimensions));
	if (!_rctx)
		log.fatal ("Failed to create RML context {}", ctxName);
}

std::unique_ptr<Context> Context::make (const std::string& ctxName, const Vec2D<int>& dimensions) {
	return std::make_unique<Context> (ctxName, dimensions);
}

Document* Context::add_document_from_file (const std::string& name, const std::string& path) {
	if (documents.contains (name)) return nullptr;
	documents.emplace (name, Document::make_from_file (rctx, path));
	return documents[name].get();
}

Document* Context::add_document_from_string (const std::string& name, const std::string& src) {
	if (documents.contains (name)) return nullptr;
	documents.emplace (name, Document::make_from_string (rctx, src));
	return documents[name].get();
}

Model* Context::add_model (const std::string& name, int flags) {
	if (models.contains (name)) return nullptr;
	models.emplace (name, Model::make (rctx, name, flags, &dataTypeRegister));
	return models[name].get();
}

}
