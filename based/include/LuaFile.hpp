#pragma once

#include <sol/sol.hpp>

namespace Based {
namespace Lua {

class File {
	std::string name;
public:
	sol::state state;

	File (const std::string &filename);
};

}
}
