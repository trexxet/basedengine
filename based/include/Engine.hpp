#pragma once

#include <memory>

#include "EngineClient.hpp"
#include "Scene.hpp"

namespace Based {

class Engine {
	bool tickUpdate ();
public:
	std::unique_ptr<EngineClient> client {nullptr};
	SceneManager sceneManager;

	void enable_client ();
	bool tick ();
};

}
