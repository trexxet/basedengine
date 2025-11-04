#pragma once

#include <print>

#include "Defs.hpp"
#include "Logger.hpp"
#include "Thread/Thread.hpp"

namespace Based {

// Logger wrapper that prepends message with <current thread name>
struct BASED_API ThreadLogger {
	template<typename... _Args>
	inline void write (std::format_string<_Args...> __fmt, _Args&&... __args) {
		if (ThreadManager::current) {
			std::lock_guard lock (log.mtx);
			log.write ("<{}> {}", ThreadManager::current, std::format (__fmt, std::forward<_Args>(__args)...));
		}
	}

	template<typename... _Args>
	inline void write_depth (uint8_t depth, std::format_string<_Args...> __fmt, _Args&&... __args) {
		if (ThreadManager::current) {
			std::lock_guard lock (log.mtx);
			log.write_depth (depth, "<{}> {}", ThreadManager::current, std::format (__fmt, std::forward<_Args>(__args)...));
		}
	}

	template<typename... _Args>
	inline void warn (std::format_string<_Args...> __fmt, _Args&&... __args) {
		if (ThreadManager::current) {
			std::lock_guard lock (log.mtx);
			log.warn ("<{}> {}", ThreadManager::current, std::format (__fmt, std::forward<_Args>(__args)...));
		}
	}

	template<typename... _Args> [[noreturn]]
	inline void fatal (std::format_string<_Args...> __fmt, _Args&&... __args) {
		if (ThreadManager::current) {
			std::lock_guard lock (log.mtx);
			log.fatal ("<{}> {}", ThreadManager::current, std::format (__fmt, std::forward<_Args>(__args)...));
		}
	}

	ThreadLogger() = default;

	BASED_CLASS_NO_COPY_MOVE (ThreadLogger);
};

#ifndef _BASED_GLOBAL_IMPLEMENT
BASED_API extern ThreadLogger threadlog;
#endif

}
