#include "LuaFile.hpp"
#include "Logger.hpp"

namespace Based {
namespace Lua {

File::File (const std::string &filename) : name(filename) {
	state.open_libraries (sol::lib::base);
	try {
		state.safe_script_file (filename);
	}
	catch (const sol::error& e) {
		log.fatal ("Error while executing Lua file {}: {}", filename, e.what());
	}
	log.write ("Loaded Lua file {}", filename);
}

}
}
