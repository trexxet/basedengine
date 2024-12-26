#include "Logger.hpp"
#include "EngineClient.hpp"

#include <SDL3/SDL_main.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main(int argv, char** args) {
	Based::log.console = true;
	Based::log.write("Demo 01-helloworld");

	Based::EngineClient client;
	client.create_window("Based hello world", WINDOW_WIDTH, WINDOW_HEIGHT);

	return 0;
}
