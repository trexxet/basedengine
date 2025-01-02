#pragma once

#include <memory>

namespace Based {

class Engine {
	friend class EngineClient;
	bool hasClient = false;
};

#ifndef _BASED_IMPLEMENT
extern Engine engine;
#endif

}
