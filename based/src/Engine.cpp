#include "Engine.hpp"

#include <sol/version.hpp>
#include <lua.h>

#define _BASED_IMPLEMENT
#include "Logger.hpp"
#undef _BASED_IMPLEMENT

namespace Based {

Logger log ("log.txt");

Engine::Engine () {
	log.write_inc ("Based Engine {}", BASED_VERSION);
	log.write_dec ("Using {}, Sol {}", LUA_RELEASE, SOL_VERSION_STRING);
}

void Engine::enable_client () {
	if (client)
		log.fatal ("Can't create multiple clients!");
	client = std::make_unique<EngineClient>(this);
	log.write ("Client enabled");
}

bool Engine::tick () {
	bool keepRunning = true;
	if (client)
		keepRunning = client->tickEvents();

	if (keepRunning)
		keepRunning = tickUpdate();

	if (client && keepRunning)
		client->tickRender();

	return keepRunning;
}

bool Engine::tickUpdate () {
	return sceneManager.update();
}

}
