#pragma once

#include <stop_token>

#include "Defs.hpp"

namespace Based {

class Engine;

class BASED_API Console {
	Engine& engine;

	void run (const std::stop_token& stop);
public:
	Console (Engine& engine);

	BASED_CLASS_NO_COPY_MOVE (Console);
};

}
