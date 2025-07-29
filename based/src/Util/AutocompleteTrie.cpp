#include "Util/AutocompleteTrie.hpp"

#include "Util/Strings.hpp"

namespace Based::Type {

void AutocompleteTrie::add (std::string_view newValue) {
	std::string_view commonPrefix;
	size_t commonPrefixNodePos = 0;

	// 0. Inserting is allowed into the non-terminal nodes; empty value is allowed only for the root node
	if (newValue.empty() || terminal)
		return;

	// 1. Find the longest common prefix
	for (size_t i = 0; i < nodes.size(); i++) {
		const AutocompleteTrie& node = nodes[i];
		if (node.value.size() <= commonPrefix.size())
			continue;
		std::string_view inodeCommonPrefix = Util::common_prefix (node.value, newValue);
		if (inodeCommonPrefix.size() > commonPrefix.size()) {
			commonPrefix = inodeCommonPrefix;
			commonPrefixNodePos = i;
			if (commonPrefix == newValue)
				break;
		}
	}

	// 2. If the common prefix is same as the current non-terminal, insert a new one here.
	if (commonPrefix == value) {
		nodes.emplace_back (newValue, true);
		return;
	}

	// 3. If the prefix is common with a non-terminal node, insert a new one there
	if (!nodes[commonPrefixNodePos].terminal) {
		nodes[commonPrefixNodePos].add (newValue);
		return;
	}

	// 4. If the prefix is common with a terminal node and differs from a parent non-terminal, make a new non-terminal node
	// containing the common prefix, move the terminal node to it and insert a new one to the created non-terminal node.
	AutocompleteTrie tmpNode (commonPrefix);
	std::swap (tmpNode, nodes[commonPrefixNodePos]);
	nodes[commonPrefixNodePos].nodes.emplace_back (std::move (tmpNode));
	nodes[commonPrefixNodePos].nodes.emplace_back (newValue, true);
}

void AutocompleteTrie::match (std::string_view prefix, MatchList& result) const {
	if (!terminal && prefix.starts_with (value)) {
		for (const AutocompleteTrie& node : nodes)
			node.match (prefix, result);
	} else if (value.starts_with (prefix)) {
		result.push_back (this);
	}
}

}
