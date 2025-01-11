#pragma once

#include <memory>

#include "EngineClient.hpp"
#include "Scene.hpp"

namespace Based {

class Engine {
	friend class EngineClient;
	bool tickUpdate ();
public:
	std::unique_ptr<EngineClient> client {nullptr};
	SceneManager sceneManager;

	Engine ();

	void enable_client ();
	bool tick ();
};

}
