#pragma once

#include <memory>

#include "EngineClient.hpp"
#include "Defs.hpp"
#include "Scene.hpp"
#include "Util/TPS.hpp"

namespace Based {

class Engine {
	friend class EngineClient;
	void tick_update ();
public:
	std::unique_ptr<EngineClient> client {nullptr};

	SceneManager sceneManager;

	Engine ();

	void enable_client ();
	bool tick ();
	void stop ();

	Util::TPSCounter tps;

	BASED_CLASS_NO_COPY_MOVE (Engine);
};

}
