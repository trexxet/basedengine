#pragma once

#include <sol/sol.hpp>

namespace Based {
namespace Lua {

class File {
	std::string name;
	sol::state state;
public:
	File (const std::string &filename);

	static File loadIfExists (const std::string &filename,
	                          const std::string &fallbackFilename);

	auto operator[] (const std::string& key) {
		return state[key];
	}
};

}
}
