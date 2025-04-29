#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/ElementDocument.h>

namespace Based::RML {

class Document {
	bool fromString;
public:
	Rml::Context* rctx = nullptr;
	Rml::ElementDocument* doc = nullptr;

	Document () = delete;
	Document (Rml::Context* rctx, bool fromString);

	void load (const std::string& source);

	/// @brief Create and load RmlDocument from file
	static std::unique_ptr<Document> make_from_file (Rml::Context* rctx, const std::string& path);
	/// @brief Create and load RmlDocument from source string
	static std::unique_ptr<Document> make_from_string (Rml::Context* rctx, const std::string& src);
};

using DocumentStorage = std::unique_ptr<Document>;
using DocumentMap = std::unordered_map<std::string, DocumentStorage>;
using DocumentHandle = Document *;

}
