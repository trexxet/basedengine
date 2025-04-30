/* Demo 02 - Image and Shader 
 * 
 * Demonstrates loading a texture, creating a graphical polygon,
 * using of a built-in shader and rendering a picture.
 * 
 * (c) trexxet 2025
 */

#include <SDL3/SDL_main.h>

#include "Engine.hpp"
#include "EngineClient.hpp"
#include "Logger.hpp"
#include "Scene.hpp"
#include "GL/Shader.hpp"

#include "config.hpp"
#include "sceneMain.hpp"

int main (int argv, char** args) {
	try {
		Based::log.console = true;
		Based::log.write (DEMO_NAME);

		Based::Engine engine;
		engine.enable_client();

		// For this demo, we are using a simple Lua config with static structure
		Config config;
		config.load (CONFIG_PATH);

		engine.client->create_window (DEMO_NAME, config.windowSize);
		Based::GL::Default::shaders.init();

		SceneMain sceneMain (&engine, config);
		engine.sceneManager.schedule_next (&sceneMain);

		Based::log.write ("Starting main loop");
		while (engine.tick());
	} catch (...) {
		Based::log.write ("Terminating due to exception");
	}

	return 0;
}
