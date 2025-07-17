#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/DataModelHandle.h>
#include <RmlUi/Core/DataTypeRegister.h>

#include "Defs.hpp"
#include "RML/Ids.hpp"

namespace Based::RML {

enum BindTypes {
	All = 0xFF,
	Geometry = 0x01
};

class BASED_API Model {
	void bindBasedTypes (BindTypes types);
public:
	std::string name;

	Rml::Context* rctx = nullptr;
	Rml::DataModelConstructor constructor;
	Rml::DataModelHandle model;

	Model (Rml::Context* rctx, const std::string& name, int flags = 0,
	       Rml::DataTypeRegister* dataTypeRegister = nullptr);

	static std::unique_ptr<Model> make (Rml::Context* rctx, const std::string& name, int flags = 0,
	                                    Rml::DataTypeRegister* dataTypeRegister = nullptr);

	BASED_CLASS_NO_COPY_DEFAULT_MOVE (Model);
};

using ModelStorage = std::unique_ptr<Model>;
using ModelMap = std::unordered_map<std::string, ModelStorage>;
using ModelHandle = Model *;

}
