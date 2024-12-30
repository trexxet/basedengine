#pragma once

#include <string>
#include <sol/sol.hpp>

namespace Based::Lua {

enum BindTypes {
	All = 0xFF,
	Geometry = 0x01
};

class File {
	std::string name;

public:
	File (const std::string &filename, int flags = 0);
	
	sol::state state;

	static File loadIfExists (const std::string &filename,
	                          const std::string &fallbackFilename,
	                          int flags = 0);

	auto operator[] (const std::string& key) {
		return state[key];
	}

	template<class Bindable, typename... _Args>
	void bind (const std::string& name, _Args&&... args) {
		state[name] = Bindable (this, std::forward<_Args> (args)...);
	}

	void bindBasedTypes (BindTypes types);
};

}
