#include "LuaFile.hpp"

#include <filesystem>

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

	log.write ("Loaded Lua file {}", filename);
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
		// Point2D, Size2D
		auto bindGeometryP2D = [this] <template <typename> class C1, typename C2> (const std::string& name) {
			this->state.new_usertype<C1<C2>>(name, sol::constructors<
				C1<C2>(),
				C1<C2>(C2, C2)>()
			);
		};
		bindGeometryP2D.operator()<Point2D, int> ("Point2D");
		bindGeometryP2D.operator()<Size2D, int> ("Size2D");
		bindGeometryP2D.operator()<Point2D, double> ("Point2Dd");
		bindGeometryP2D.operator()<Size2D, double> ("Size2Dd");
		// Rect2D
		auto bindGeometryR2D = [this] <typename C> (const std::string& name) {
			this->state.new_usertype<Rect2D<C>>(name, sol::constructors<
				Rect2D<C>(),
				Rect2D<C>(C, C, C, C),
				Rect2D<C>(Size2D<C>),
				Rect2D<C>(C, C, Size2D<C>),
				Rect2D<C>(Point2D<C>, Size2D<C>)>(),
				"centrify", &Rect2D<C>::template centrify<C>
			);
		};
		bindGeometryR2D.operator()<int> ("Rect2D");
		bindGeometryR2D.operator()<double> ("Rect2Dd");
	}
}

}
