#include "Thread.hpp"

#include <exception>

#include "Logger.hpp"

#define BASED_MAIN_THREAD_NAME "based::mainThread"

namespace Based {

Thread::Thread (const std::string& name) : _name(name) {
	if (name == BASED_MAIN_THREAD_NAME) {
		ThreadManager::_current = name;
		return;
	}
	t = std::thread([this, name]() {
		ThreadManager::_current = name;
		std::unique_lock lock (mtx);
		cv.wait (lock, [this]() { return assigned || cancelled; });
		if (cancelled) return;
		if (fn) fn();
	});
}

void Thread::assign (decltype(Thread::fn) task) {
	{
		std::lock_guard lock (mtx);
		fn = std::move (task);
		assigned = true;
	}
	cv.notify_one();
}

Thread::~Thread () {
	if (name != BASED_MAIN_THREAD_NAME && t.joinable()) {
		{
			std::lock_guard lock (mtx);
			cancelled = true;
		}
		cv.notify_one();
		t.join();
		log.write ("Thread {} shutdown", name);
	}
}

thread_local std::string ThreadManager::_current;

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
