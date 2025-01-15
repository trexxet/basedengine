#pragma once

#include <concepts>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>

#include "Logger.hpp"

namespace Based {

struct Resource {
	virtual void load (const std::string& path) = 0;
	virtual bool prepare () = 0;
	virtual void unload () = 0;

	bool ready = false;
	Resource() = default;
	Resource* get () { return this; }
};

template <class T>
concept ResourceClass = std::derived_from<T, Resource>;

using ResourceMap = std::unordered_map<std::string, std::unique_ptr<Resource>>;

class ResourceManager {
	ResourceMap resourceMap;
public:
	template <ResourceClass T>
	bool load (const std::string& name, const std::string& path) {
		if (resourceMap.contains (name))
			return true;
		try {
			resourceMap.emplace (name, std::make_unique<T>());
			resourceMap[name]->load (path);
		}
		catch (const std::exception &e) {
			log.warn ("Failed to load resource {}: {}", name, path);
			return false;
		}
		return true;
	}

	template <ResourceClass T>
	bool prepare (const std::string& name) {
		if (!resourceMap.contains (name)) [[unlikely]] {
			log.warn ("Failed to prepare resource {}: not loaded", name);
			return false;
		}
		return resourceMap[name]->prepare();
	}

	template <ResourceClass T>
	bool unload (const std::string& name) {
		if (!resourceMap.contains (name)) [[unlikely]] {
			log.warn ("Failed to unload resource {}: not loaded", name);
			return false;
		}
		try {
			resourceMap[name]->unload();
		}
		catch (const std::exception &e) {
			log.warn ("Failed to unload resource {}", name);
			return false;
		}
		resourceMap.erase (name);
	}

	template <ResourceClass T>
	T* operator[] (const std::string& name) {
		return static_cast<T*> (resourceMap[name].get());
	}
};

}
