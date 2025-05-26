/* Demo 01 - Hello World 
 * 
 * Demonstrates initialization, logging, window creation, loading Lua config,
 * and basic event handling.
 * 
 * (c) trexxet 2025
 */

/* You still need to include SDL_main.h to the source file
 * containing your `int main (int argv, char** argc)`. Note
 * that you need to declare argv and argc arguments!
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
	// Note that exceptions across DLL boundaries must be caught explicitly to correctly unwind stack
	try {
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

		SceneMain sceneMain (engine, conf);
		engine.sceneManager.schedule_next (&sceneMain);

		Based::log.write ("Starting main loop");
		while (engine.tick());
	} catch (...) {
		Based::log.write ("Terminating due to exception");
	}

	return 0;
}
