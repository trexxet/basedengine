#pragma once

#include <fstream>

#include "Logger.hpp"

namespace Based {

/**
 * Load file contents into string
 * \param path file path
 * \return string containing loaded file, or empty if failed
 */
std::string loadFileToString (const std::string& path) {
	std::string string;
	std::ifstream file (path, std::ios::in | std::ios::binary);
	if (file) {
		file.seekg (0, std::ios::end);
		size_t size = (size_t) file.tellg();
		string.resize (size);
		file.seekg (0, std::ios::beg);
		file.read (&string[0], size);
		file.close();
		log.write ("File loaded: {}", path);
	} else {
		log.warn ("Error loading file: {}", path);
	}
	return string;
}

}
