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
	bool keepRunning;

	if (client)
		keepRunning = client->tick();
	else
		keepRunning = tickUpdate();

	sceneManager.commit_scheduled();

	return keepRunning;
}

bool Engine::tickUpdate () {
	return sceneManager.update();
}

}
