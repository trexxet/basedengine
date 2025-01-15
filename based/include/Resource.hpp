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

	bool loaded = false;
	bool ready = false;

	Resource() = default;
	inline Resource* get () { return this; }
};

template <class T>
concept ResourceClass = std::derived_from<T, Resource>;

using ResourceMap = std::unordered_map<std::string, std::unique_ptr<Resource>>;

class ResourceManager {
	ResourceMap resourceMap;
public:
/**
 * Synchronously load single resource
 * \param name name of the resource
 * \param path path to resource file
 */
	template <ResourceClass T, typename... _Args>
	bool load (const std::string& name, const std::string& path, _Args&&... args) {
		if (resourceMap.contains (name))
			return true;
		try {
			resourceMap.emplace (name, std::make_unique<T>(std::forward<_Args> (args)...));
			resourceMap[name]->load (path);
		}
		catch (const std::exception &e) {
			log.warn ("Failed to load resource {}", name);
			return false;
		}
		log.write ("Loaded resource {}", name);
		return true;
	}

/**
 * Synchronously prepare single loaded resource for use
 * \param name name of the resource
 */
	bool prepare (const std::string& name) {
		if (!resourceMap.contains (name) || !resourceMap[name]->loaded) [[unlikely]] {
			log.warn ("Failed to prepare resource {}: not loaded", name);
			return false;
		}
		return resourceMap[name]->prepare();
	}

/**
 * Synchronously unload single resource
 * \param name name of the resource
 */
	bool unload (const std::string& name) {
		if (!resourceMap.contains (name) || !resourceMap[name]->loaded) [[unlikely]] {
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

/**
 * Get pointer to loaded resource
 * \param name name of the resource
 */
	template <ResourceClass T>
	inline T* get (const std::string& name) {
		return static_cast<T*> (resourceMap[name].get());
	}

	ResourceManager () = default;
	ResourceManager (const ResourceManager&) = delete;
	ResourceManager& operator= (const ResourceManager&) = delete;
	ResourceManager (ResourceManager&&) = delete;
	ResourceManager& operator= (ResourceManager&&) = delete;
	~ResourceManager () {
		for (auto & [name, res] : resourceMap) {
			try {
				res->unload();
			}
			catch (const std::exception &e) {
				log.warn ("Failed to unload resource {}", name);
			}
		}
		resourceMap.clear();
	}
};

}
