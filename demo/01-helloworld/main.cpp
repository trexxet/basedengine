#include "Logger.hpp"

#include <SDL3/SDL_main.h>

int main(int argv, char** args) {
	Based::log.console = true;
	Based::log.write("Demo 01-helloworld");
	return 0;
}
