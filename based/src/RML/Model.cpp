#include "RML/Model.hpp"

#include "Geometry.hpp"
#include "Logger.hpp"

namespace Based::RML {

Model::Model (Rml::Context* rctx, const std::string& name, int flags, Rml::DataTypeRegister* dataTypeRegister)
	: name (name), rctx (rctx)
{
	if (!rctx)
		log.fatal ("Failed to create RML Data Model {}: null context", name);
	
	constructor = rctx->CreateDataModel (name, dataTypeRegister);
	if (!constructor)
		log.fatal ("Failed to create RML Data Model Constructor {}", name);

	if (flags & BindTypes::All)
		bindBasedTypes ((BindTypes)(flags & BindTypes::All));

	model = constructor.GetModelHandle();
}

std::unique_ptr<Model> Model::make (Rml::Context* rctx, const std::string& name, int flags,
Rml::DataTypeRegister* dataTypeRegister) {
	return std::make_unique<Model> (rctx, name, flags, dataTypeRegister);
}

void Model::bindBasedTypes (BindTypes types) {
	auto bindBasedTemplatedType = [this] <typename T> (auto&& bind_lambda, const std::string& name) {
		bool fieldsOk = true;
		if (!bind_lambda.template operator()<T> (fieldsOk))
			log.fatal ("Failed to bind {} to RML", name);
		if (!fieldsOk)
			log.fatal ("Failed to bind {} fields to RML", name);
	};

#define BIND_FIELD(f, c) RegisterMember (#f, &c::f)
	if (types & BindTypes::Geometry) {
		// Rect2D
		auto bindGeometryRect2D = [this] <typename T> (bool& fieldsOk) -> bool {
			if (auto handle = this->constructor.RegisterStruct<Rect2D<T>>()) {
#define BIND_RECT2D_FIELD(f) fieldsOk &= handle.BIND_FIELD (f, Rect2D<T>)
				BIND_RECT2D_FIELD(x);
				BIND_RECT2D_FIELD(y);
				BIND_RECT2D_FIELD(w);
				BIND_RECT2D_FIELD(h);
#undef BIND_RECT2D_FIELD
				return true;
			}
			return false;
		};
		bindBasedTemplatedType.operator()<int> (bindGeometryRect2D, "Rect2D<int>");
		bindBasedTemplatedType.operator()<float> (bindGeometryRect2D, "Rect2D<float>");
	}
#undef BIND_FIELD
}

}

#define BASED_RML_ID_DEF(C) template class Rml::Family<C>

BASED_RML_ID_DEF(Based::Rect2D<int>);

#undef BASED_RML_ID_DEF
