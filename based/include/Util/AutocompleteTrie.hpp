#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "Defs.hpp"

namespace Based::Type {

struct BASED_API AutocompleteTrie {
	using MatchList = std::vector<const AutocompleteTrie*>;

	bool terminal = false;
	std::string value;
	std::vector<AutocompleteTrie> nodes;

	/// @brief Add a new terminal node and rearrange prefixes if needed
	void add (std::string_view newValue);
	/// @brief Get a list of nodes that match the prefix
	void match (std::string_view prefix, MatchList& result) const;
	/// @brief Get a list of nodes that match the prefix
	inline MatchList match (std::string_view prefix) const {
		MatchList result;
		match (prefix, result);
		return result;
	}

	AutocompleteTrie () = default;
	AutocompleteTrie (std::string_view value, bool terminal = false) : value (value), terminal (terminal) { }
};

}
