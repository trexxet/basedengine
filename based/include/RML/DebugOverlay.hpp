#pragma once

#include <RML/Context.hpp>

namespace Based {
	class Engine;
}
using Based::Engine;

namespace Based::RML {

class DebugOverlay {
	Engine* engine;
	ContextHandle rctx;
public:
	DebugOverlay (Engine* engine, ContextStorage& rctx);
};

}
