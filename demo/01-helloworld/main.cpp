#include "Logger.hpp"

#include <SDL3/SDL_main.h>

#include "Engine.hpp"
#include "EngineClient.hpp"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(int argv, char** args) {
	Based::log.console = true;
	Based::log.write ("Demo 01-helloworld");

	Based::Engine engine;

	Based::EngineClient client;
	client.create_window ("Based hello world", WINDOW_WIDTH, WINDOW_HEIGHT);

	return 0;
}
