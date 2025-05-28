#pragma once

#include <memory>

#include "EngineClient.hpp"
#include "Defs.hpp"
#include "Scene.hpp"

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

	BASED_CLASS_NO_COPY_MOVE (Engine);
};

}
