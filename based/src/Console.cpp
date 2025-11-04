#include "Console.hpp"

#include "Engine.hpp"
#include "Thread.hpp"
#include "ThreadNames.hpp"

namespace Based {

Console::Console (Engine& engine) : engine (engine) {
	engine.threadManager.create (BASED_CONSOLE_THREAD_NAME);
}

}
