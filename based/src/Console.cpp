#include "Console.hpp"

#include "Engine.hpp"
#include "Thread/Thread.hpp"
#include "Thread/Names.hpp"

namespace Based {

Console::Console (Engine& engine) : engine (engine) {
	engine.threadManager.create (BASED_CONSOLE_THREAD_NAME);
}

}
