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
		auto bindGeometry = [this] <template <typename> class C1, typename C2> (const std::string& name) {
			this->state.new_usertype<C1<C2>>(name, sol::constructors<C1<C2>(C2, C2)>());
		};
		bindGeometry.operator()<Point2D, int> ("Point2D");
		bindGeometry.operator()<Size2D, int> ("Size2D");
		bindGeometry.operator()<Point2D, double> ("Point2Dd");
		bindGeometry.operator()<Size2D, double> ("Size2Dd");
	}
}

}
