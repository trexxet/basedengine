#include <RML/DebugOverlay.hpp>

#include <Engine.hpp>

namespace Based::RML {

DebugOverlay::DebugOverlay (Engine* engine, ContextStorage& rctx) : engine(engine), rctx(rctx.get()) {
	if (!rctx) [[unlikely]]
		log.fatal ("Can't set RML context for debug overlay!");
}

}
