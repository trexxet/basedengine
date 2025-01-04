#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <print>
#include <stdexcept>

namespace Based {

class Logger {
	std::string name;
	FILE *logfile;
	uint8_t depth = 0;
	
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
		return std::format ("{} |    {}{}{}\n",
			get_date_time(),
			std::string (depth * 8, ' '),
			prefix,
			std::format (__fmt, std::forward<_Args>(__args)...));
	}

	inline void dowrite (const std::string msg, bool to_stderr = false) {
		std::print (logfile, "{}", msg);
		if (console && !to_stderr) std::print ("{}", msg);
		if (to_stderr) std::print(stderr, "{}", msg);
	}

public:
	Logger (const char *filename) : name(filename) {
		logfile = fopen (filename, "w");
		if (!logfile)
			throw std::runtime_error (make_message ("", "Can't open {} for write", name));
		dowrite (make_message ("", "Created log {}", name));
	}

	~Logger () {
		depth = 0;
		dowrite (make_message ("", "Closing log {}", name));
		fclose (logfile);
	}

	bool console = false;
	inline void inc_depth () { if (depth < 7) depth++; }
	inline void dec_depth () { if (depth > 0) depth--; }

#define DOWRITE { dowrite (make_message ("", __fmt, std::forward<_Args>(__args)...)); }
#define LOGGER_FUNCTION(name) template<typename... _Args> void name (std::format_string<_Args...> __fmt, _Args&&... __args)

	LOGGER_FUNCTION(write)  {
		DOWRITE;
	}

	LOGGER_FUNCTION(write_inc)  { // print and increase depth
		DOWRITE;
		inc_depth();
	}

	LOGGER_FUNCTION(inc_write)  { // increase depth and print
		inc_depth();
		DOWRITE;
	}

	LOGGER_FUNCTION(write_dec)  { // print and decrease depth
		DOWRITE;
		dec_depth();
	}

	LOGGER_FUNCTION(dec_write)  { // decrease depth and print
		dec_depth();
		DOWRITE;
	}

	LOGGER_FUNCTION(warn) {
		auto depth_save = depth;
		depth = 0;
		dowrite (make_message ("WARN: ", __fmt, std::forward<_Args>(__args)...), true);
		depth = depth_save;
	}

	LOGGER_FUNCTION(fatal) {
		depth = 0;
		std::string msg = make_message ("ERROR: ", __fmt, std::forward<_Args>(__args)...);
		dowrite (msg, true);
		throw std::runtime_error (msg);
	}

#undef LOGGER_FUNCTION
#undef DOWRITE

};

#ifndef _BASED_IMPLEMENT
extern Logger log;
#endif

}
