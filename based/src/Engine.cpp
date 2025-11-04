#include "Engine.hpp"

#include <sol/version.hpp>
#include <lua.h>

#define _BASED_GLOBAL_IMPLEMENT
#include "Logger.hpp"
#include "Thread/ThreadLogger.hpp"
#undef _BASED_GLOBAL_IMPLEMENT

namespace Based {

Logger log ("log.txt");
ThreadLogger threadlog;

Engine::Engine () : console (*this) {
	log.write ("Based Engine {}", BASED_VERSION);
	log.write_depth (1, "Using {}, Sol {}", LUA_RELEASE, SOL_VERSION_STRING);
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
