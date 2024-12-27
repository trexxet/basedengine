#pragma once

#include <memory>

namespace Based {

class Engine {
	struct _Lua {
		_Lua ();
		~_Lua ();
	};
	using Lua = std::unique_ptr<_Lua>;
	Lua lua;
};

}
