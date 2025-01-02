#include "Logger.hpp"

#include <SDL3/SDL_main.h>

#include "Engine.hpp"
#include "EngineClient.hpp"
#include "Geometry.hpp"
#include "LuaFile.hpp"
#include "Scene.hpp"

#include "sceneMain.hpp"

#define CONFIG_PATH "../cfg/01-helloworld/config.lua"

int main (int argv, char** args) {
	Based::log.console = true;
	Based::log.write ("Demo 01-helloworld");

	Based::Lua::File conf (CONFIG_PATH, Based::Lua::BindTypes::Geometry);
	Based::Size2D<int> window = conf["window"]; 

	Based::EngineClient client;
	client.create_window ("Based hello world", window.width, window.height);

	SceneMain sceneMain;
	Based::sceneManager.currentScene = &sceneMain;

	Based::sceneManager.update();
	Based::sceneManager.render();
	Based::sceneManager.handleEvents();

	return 0;
}
