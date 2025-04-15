/* Demo 03 - Console & Lua
 * 
 * TODO: proper description
 * 
 * (c) trexxet 2025
 */

#include <SDL3/SDL_main.h>

#include "Engine.hpp"
#include "EngineClient.hpp"
#include "Geometry.hpp"
#include "Logger.hpp"
#include "LuaFile.hpp"
#include "Scene.hpp"

#include "sceneMain.hpp"

int main (int argv, char** args) {
	Based::log.console = true;
	Based::log.write (DEMO_NAME);

	Based::Engine engine;
	if (ENABLE_CLIENT)
		engine.enable_client();

	Based::Lua::File conf (CONFIG_PATH, Based::Lua::BindTypes::Geometry);
	if (ENABLE_CLIENT) {
		Based::Vec2D<int> windowSize = conf["window"]; 
		engine.client->create_window (DEMO_NAME, windowSize);
	}

	SceneMain sceneMain (&engine, conf);
	engine.sceneManager.schedule_next (&sceneMain);

	Based::log.write ("Starting main loop");
	while (engine.tick());

	return 0;
}
