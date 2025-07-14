#include "Engine.hpp"

#include <SDL3/SDL_timer.h>

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

uint16_t Engine::get_tps () {
	static thread_local uint16_t tps = 0;
	static thread_local uint16_t ticks = 0;
	static thread_local uint64_t measure_last = 0;
	const uint64_t one_second = 1000; // in ms

	ticks++;

	uint64_t measure = SDL_GetTicks();
	if (measure - measure_last >= one_second) {
		measure_last = measure;
		tps = ticks;
		ticks = 0;
	}

	return tps;
}

}
