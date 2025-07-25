#include "Engine.hpp"

#include <sol/version.hpp>
#include <lua.h>

#define _BASED_LOGGER_IMPLEMENT
#include "Logger.hpp"
#undef _BASED_LOGGER_IMPLEMENT

namespace Based {

Logger log ("log.txt");

Engine::Engine () {
	log.write_inc ("Based Engine {}", BASED_VERSION);
	log.write_dec ("Using {}, Sol {}", LUA_RELEASE, SOL_VERSION_STRING);
}

void Engine::enable_client () {
	if (client)
		log.fatal ("Can't create multiple clients!");
	client = std::make_unique<EngineClient> (*this);
	log.write ("Client enabled");
}

bool Engine::tick () {
	tps.tick();
	if (client)
		client->tick();
	else
		tick_update();
	return sceneManager.commit_scheduled();
}

void Engine::tick_update () {
	sceneManager.update();
}

void Engine::stop () {
	sceneManager.schedule_next (nullptr);
}

}
