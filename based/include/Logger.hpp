#pragma once

// TODO: make Logger console flag more thread-safe

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <print>
#include <stdexcept>

#include "Defs.hpp"

namespace Based {

class BASED_API Logger {
	std::string name;
	FILE *logfile;

	// std::chrono is absolutely retarded under mingw
	inline std::string get_date_time () {
		char timestrbuf[32] = {0};
		struct tm tmbuf;
		time_t now = time (NULL);
		localtime_s (&tmbuf, &now);
		strftime (timestrbuf, sizeof (timestrbuf), "%d-%m-%Y %X", &tmbuf);
		return std::string {timestrbuf};
	}

	template<typename... _Args>
	std::string make_message (const std::string& prefix, std::format_string<_Args...> __fmt, _Args&&... __args) {
		return std::format ("{} |    {}{}\n",
			get_date_time(),
			prefix,
			std::format (__fmt, std::forward<_Args>(__args)...));
	}

	inline void dowrite (const std::string msg, bool to_stderr = false) {
		std::print (logfile, "{}", msg);
		if (console && !to_stderr) std::print ("{}", msg);
		if (to_stderr) std::print(stderr, "{}", msg);
	}

public:
	bool console = false;

	Logger (const char *filename) : name(filename) {
		logfile = fopen (filename, "w");
		if (!logfile)
			throw std::runtime_error (make_message ("", "Can't open {} for write", name));
		dowrite (make_message ("", "Created log {}", name));
	}

	~Logger () {
		dowrite (make_message ("", "Closing log {}", name));
		fclose (logfile);
	}

	template<typename... _Args>
	void write (std::format_string<_Args...> __fmt, _Args&&... __args) {
		dowrite (make_message ("", __fmt, std::forward<_Args>(__args)...));
	}

	template<typename... _Args>
	void write_depth (uint8_t depth, std::format_string<_Args...> __fmt, _Args&&... __args) {
		dowrite (make_message (std::string (depth * 8, ' '), __fmt, std::forward<_Args>(__args)...));
	}

	template<typename... _Args>
	void warn (std::format_string<_Args...> __fmt, _Args&&... __args) {
		dowrite (make_message ("WARN: ", __fmt, std::forward<_Args>(__args)...), true);
	}

	template<typename... _Args> [[noreturn]]
	void fatal (std::format_string<_Args...> __fmt, _Args&&... __args) {
		std::string msg = std::format (__fmt, std::forward<_Args>(__args)...);
		dowrite (make_message ("ERROR: ", "{}", msg), true);
		throw std::runtime_error (msg);
	}

	BASED_CLASS_NO_COPY_MOVE (Logger);
};

#ifndef _BASED_GLOBAL_IMPLEMENT
BASED_API extern Logger log;
#endif

}
