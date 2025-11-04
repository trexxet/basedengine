#include "Thread/Thread.hpp"

#include <exception>
#include <stop_token>

#include "Logger.hpp"
#include "Thread/Names.hpp"

namespace Based {

Thread::Thread (const std::string& name) : _name(name) {
	if (name == BASED_MAIN_THREAD_NAME) {
		ThreadManager::current = _name.c_str();
		return;
	}
	t = std::jthread([this, name] (std::stop_token stop) {
		ThreadManager::current = _name.c_str();

		std::unique_lock lock (mtx);
		cv.wait (lock, [this, stop]() { return fn || stop.stop_requested(); });
		if (stop.stop_requested()) return;

		if (fn) fn (stop);
	});
}

void Thread::assign (decltype(Thread::fn) task) {
	{
		std::lock_guard lock (mtx);
		fn = std::move (task);
	}
	cv.notify_one();
}

Thread::~Thread () {
	if (name != BASED_MAIN_THREAD_NAME) {
		t.request_stop();
		cv.notify_one();
		log.write ("Thread {} shutdown", name);
	}
}

thread_local const char* ThreadManager::current = nullptr;

Thread& ThreadManager::create (const std::string& name) {
	try {
		auto [it, emplaced] = threadMap.try_emplace (name, name);
		if (!emplaced)
			log.fatal ("Thread {} already exists", name);
		log.write ("Created thread {}", name);
		return it->second;
	}
	catch (const std::exception &e) {
		log.fatal ("Failed to create thread {}", name);
	}
}

ThreadManager::ThreadManager () {
	create (BASED_MAIN_THREAD_NAME);
}

}
