#include "Logger.hpp"

#include <SDL3/SDL_main.h>

#include "Engine.hpp"
#include "EngineClient.hpp"
#include "Geometry.hpp"
#include "LuaFile.hpp"
#include "Scene.hpp"

#include "sceneMain.hpp"

int main (int argv, char** args) {
	Based::log.console = true;
	Based::log.write (DEMO_NAME);

	Based::Engine engine;
	engine.enable_client();

	Based::Lua::File conf (CONFIG_PATH, Based::Lua::BindTypes::Geometry);
	Based::Size2D<int> windowSize = conf["window"]; 
	engine.client->create_window (DEMO_NAME, windowSize);

	SceneMain sceneMain (engine.client->window(), conf);
	engine.sceneManager.currentScene = &sceneMain;

	Based::log.write ("Starting main loop");
	while (engine.tick());

	return 0;
}

