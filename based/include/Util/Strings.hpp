#pragma once

#include <string_view>

namespace Based::Util {

/// @brief Return common prefix of two strings
inline std::string_view common_prefix (std::string_view a, std::string_view b) {
	size_t len = 0;
	while (len < a.size() && len < b.size() && a[len] == b[len])
		len++;
	return a.substr (0, len);
}

}
