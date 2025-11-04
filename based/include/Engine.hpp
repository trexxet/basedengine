#pragma once

#include <memory>

#include "Console.hpp"
#include "Defs.hpp"
#include "EngineClient.hpp"
#include "Scene.hpp"
#include "Thread/Thread.hpp"
#include "Util/TPS.hpp"

namespace Based {

class BASED_API Engine {
	friend class EngineClient;
	void tick_update ();
public:
	std::unique_ptr<EngineClient> client {nullptr};

	SceneManager sceneManager;
	ThreadManager threadManager;

	Engine ();

	void enable_client ();
	bool tick ();
	void stop ();

	Util::TPSCounter tps;

	Console console;

	BASED_CLASS_NO_COPY_MOVE (Engine);
};

}
