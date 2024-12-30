#include "Logger.hpp"

#include <SDL3/SDL_main.h>

#include "Engine.hpp"
#include "EngineClient.hpp"
#include "LuaFile.hpp"
#include "Geometry.hpp"

#define CONFIG_PATH "../cfg/01-helloworld/config.lua"

int main (int argv, char** args) {
	Based::log.console = true;
	Based::log.write ("Demo 01-helloworld");

	Based::Lua::File conf (CONFIG_PATH, Based::Lua::BindTypes::Geometry);
	Based::Size2D<int> window = conf["window"]; 

	Based::Engine engine;

	Based::EngineClient client;
	client.create_window ("Based hello world", window.width, window.height);

	return 0;
}
