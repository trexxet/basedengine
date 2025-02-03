#include "LuaFile.hpp"

#include <filesystem>

#include <glad/gl.h>

#include "Geometry.hpp"
#include "Logger.hpp"

namespace Based::Lua {

File::File (const std::string &filename, int flags) : name(filename) {
	state.open_libraries (sol::lib::base);
	if (flags & BindTypes::All)
		bindBasedTypes((BindTypes)(flags & BindTypes::All));

	try {
		state.safe_script_file (filename);
	}
	catch (const sol::error& e) {
		log.fatal ("Error while executing Lua file {}: {}", filename, e.what());
	}

	log.write ("Loaded Lua file: {}", filename);
}

File File::loadIfExists (const std::string &filename,
                         const std::string &fallbackFilename,
                         int flags)
{
	if (std::filesystem::exists (filename))
		return File (filename, flags);
	else {
		log.write ("Can't find lua file {}, fallback to {}", filename, fallbackFilename);
		return File (fallbackFilename, flags);
	}
}

#define BIND_FIELD(f, c) #f, &c::f
void File::bindBasedTypes (BindTypes types) {
	if (types & BindTypes::Geometry) {
		// Vec2D
		auto bindGeometryVec2D = [this] <typename T> (const std::string& name) {
			this->state.new_usertype<Vec2D<T>>(name, sol::constructors<
				Vec2D<T>(),
				Vec2D<T>(T, T)>(),
#define BIND_VEC2D_FIELD(f) BIND_FIELD(f, Vec2D<T>)
				BIND_VEC2D_FIELD (x),
				BIND_VEC2D_FIELD (y),
				BIND_VEC2D_FIELD (s),
				BIND_VEC2D_FIELD (t)
#undef BIND_VEC2D_FIELD
			);
		};
		bindGeometryVec2D.operator()<int> ("Vec2Di");
		bindGeometryVec2D.operator()<GLfloat> ("Vec2D");
		// Rect2D
		auto bindGeometryRect2D = [this] <typename T> (const std::string& name) {
			this->state.new_usertype<Rect2D<T>>(name, sol::constructors<
				Rect2D<T>(),
				Rect2D<T>(T, T, T, T),
				Rect2D<T>(Vec2D<T>),
				Rect2D<T>(Vec2D<T>, T, T),
				Rect2D<T>(T, T, Vec2D<T>),
				Rect2D<T>(Vec2D<T>, Vec2D<T>)>(),
				"centrify", &Rect2D<T>::template centrify<T>
			);
		};
		bindGeometryRect2D.operator()<int> ("Rect2Di");
		bindGeometryRect2D.operator()<GLfloat> ("Rect2D");
		// Circle2D
		auto bindGeometryCircle2D = [this] <typename T> (const std::string& name) {
			this->state.new_usertype<Circle2D<T>>(name, sol::constructors<
				Circle2D<T>(),
				Circle2D<T>(T, T, T),
				Circle2D<T>(Vec2D<T>, T)>()
			);
		};
		bindGeometryCircle2D.operator()<GLfloat> ("Circle2D");
	}
}
#undef BIND_FIELD

}
