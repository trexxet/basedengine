#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

#include "Defs.hpp"

namespace Based {

class BASED_API Thread {
	friend class ThreadManager;
	std::string _name;

	std::function<void(const std::stop_token&)> fn;  // TODO: consider using fixed function for better performance
	std::mutex mtx;
	std::condition_variable cv;
	std::jthread t;
public:
	const decltype((_name)) name = _name;

	void assign (decltype(fn));

	Thread () = delete;
	Thread (const std::string& name);
	~Thread ();

	BASED_CLASS_NO_COPY_MOVE (Thread);
};

using ThreadMap = std::unordered_map<std::string, Thread>;

class BASED_API ThreadManager {
	ThreadMap threadMap;
public:
	static thread_local const char* current;

	Thread& create (const std::string& name);
	inline Thread& operator[] (const std::string& name) {
		return threadMap.at(name);
	}

	ThreadManager ();

	BASED_CLASS_NO_COPY_MOVE (ThreadManager);
};

}
