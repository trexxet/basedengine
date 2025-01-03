#include "Engine.hpp"

#define _BASED_IMPLEMENT
#include "Logger.hpp"
#undef _BASED_IMPLEMENT

namespace Based {

Logger log ("log.txt");

void Engine::enable_client () {
	if (client)
		log.fatal ("Can't create multiple clients!");
	client = std::make_unique<EngineClient>(this);
}

bool Engine::tick () {
	bool keepRunning = true;
	if (client)
		keepRunning &= client->tickEvents();

	keepRunning &= tickUpdate();

	if (client)
		client->tickRender();

	return keepRunning;
}

bool Engine::tickUpdate () {
	return sceneManager.update();
}

}
