#include <string>

namespace Based {
	class Engine;
}

#define BASED_DEBUG_OVERLAY_UPDATE_CB(fn) const std::string& fn (Based::Engine& engine)

namespace Based::RML::DebugOverlay {
	BASED_DEBUG_OVERLAY_UPDATE_CB(dou_tps);
}
