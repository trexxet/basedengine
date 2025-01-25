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

void File::bindBasedTypes (BindTypes types) {
	if (types & BindTypes::Geometry) {
		// Vec2D
		auto bindGeometryVec2D = [this] <typename C> (const std::string& name) {
			this->state.new_usertype<Vec2D<C>>(name, sol::constructors<
				Vec2D<C>(),
				Vec2D<C>(C, C)>(),
#define BIND_FIELD(f) #f, &Vec2D<C>::f
				BIND_FIELD (x),
				BIND_FIELD (y),
				BIND_FIELD (s),
				BIND_FIELD (t)
#undef BIND_FIELD
			);
		};
		bindGeometryVec2D.operator()<int> ("Vec2Di");
		bindGeometryVec2D.operator()<GLfloat> ("Vec2D");
		// Rect2D
		auto bindGeometryRect2D = [this] <typename C> (const std::string& name) {
			this->state.new_usertype<Rect2D<C>>(name, sol::constructors<
				Rect2D<C>(),
				Rect2D<C>(C, C, C, C),
				Rect2D<C>(Vec2D<C>),
				Rect2D<C>(Vec2D<C>, C, C),
				Rect2D<C>(C, C, Vec2D<C>),
				Rect2D<C>(Vec2D<C>, Vec2D<C>)>(),
				"centrify", &Rect2D<C>::template centrify<C>
			);
		};
		bindGeometryRect2D.operator()<int> ("Rect2Di");
		bindGeometryRect2D.operator()<GLfloat> ("Rect2D");
	}
}

}
