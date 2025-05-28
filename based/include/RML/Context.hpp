#pragma once

#include <memory>
#include <string>

#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/DataTypeRegister.h>

#include "Geometry.hpp"
#include "RML/Document.hpp"
#include "RML/Model.hpp"

namespace Based::RML {

class Context {
	Rml::Context* _rctx = nullptr;
public:
	std::string ctxName;

	Rml::Context* const& rctx = _rctx;
	Rml::DataTypeRegister dataTypeRegister;

	Context (const std::string& ctxName, const Vec2D<int>& dimensions);

	DocumentMap documents;
	ModelMap models;

	DocumentHandle add_document_from_file (const std::string& name, const std::string& path);
	DocumentHandle add_document_from_string (const std::string& name, const std::string& src);
	ModelHandle add_model (const std::string& name, int flags = 0);

	static std::unique_ptr<Context> make (const std::string& ctxName, const Vec2D<int>& dimensions);
};

using ContextStorage = std::unique_ptr<Context>;
using ContextHandle = Context *;

}
