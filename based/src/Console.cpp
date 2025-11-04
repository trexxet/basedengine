#include "Console.hpp"

#include "Engine.hpp"
#include "Thread/Thread.hpp"
#include "Thread/ThreadLogger.hpp"
#include "Thread/Names.hpp"

namespace Based {

void Console::run (const std::stop_token& stop) {
	threadlog.write ("Running console thread.......");
}

Console::Console (Engine& engine) : engine (engine) {
	Thread& t = engine.threadManager.create (BASED_CONSOLE_THREAD_NAME);
	t.assign ([this](const std::stop_token& stop) { run (stop); });
}

}
