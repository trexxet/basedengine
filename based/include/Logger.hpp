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
	uint8_t depth = 1;
	
	// std::chrono is absolutely retarded under mingw
	inline std::string get_date_time () {
		char timestrbuf[32] = {0};
		struct tm tmbuf;
		time_t now = time (NULL);
		localtime_s (&tmbuf, &now);
		strftime (timestrbuf, sizeof(timestrbuf), "%d-%m-%Y %X", &tmbuf);
		return std::string {timestrbuf};
	}

	template<typename... _Args>
	void dowrite (std::format_string<_Args...> __fmt, _Args&&... __args) {
		std::string msg = std::format ("{} | {}{}\n",
			get_date_time(),
			std::string(depth, '\t'),
			std::format (__fmt, std::forward<_Args>(__args)...));
		std::print(logfile, "{}", msg);
		if (console) std::print("{}", msg);
	}

public:
	Logger (const char *filename) : name(filename) {
		logfile = fopen (filename, "w");
		if (!logfile)
			throw std::runtime_error(std::format("Can't open {} for write", name));
		dowrite ("Created log {}", name);
	}

	~Logger () {
		depth = 1;
		dowrite ("Closing log {}", name);
		fclose (logfile);
	}

	bool console = false;
	inline void inc_depth () { if (depth < 8) depth++; }
	inline void dec_depth () { if (depth > 1) depth--; }

#define DOWRITE { dowrite (__fmt, std::forward<_Args>(__args)...); }
#define LOGGER_FUNCTION(name) template<typename... _Args> void name (std::format_string<_Args...> __fmt, _Args&&... __args)

	LOGGER_FUNCTION(write)  {
		DOWRITE;
	}

	LOGGER_FUNCTION(write_inc)  { // print and increase depth
		DOWRITE;
		inc_depth ();
	}

	LOGGER_FUNCTION(write_dec)  { // print and decrease depth
		DOWRITE;
		dec_depth ();
	}

	LOGGER_FUNCTION(dec_write)  { // decrease depth and print
		dec_depth ();
		DOWRITE;
	}

#undef LOGGER_FUNCTION
#undef DOWRITE

};

#ifndef IMPLEMENT
extern Logger log;
#endif

}
