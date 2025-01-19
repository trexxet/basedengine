/* Resource and ResourceManager
 * 
 * Resource is a base class for objects that can be
 * loaded externally and needs their lifetime managed.
 * ResourceManager wraps management and access for multiple Resources.
 * Current list of built-in Resources:
 * * Shader
 * * ShaderProgram
 * * Texture
 * 
 * (c) trexxet 2025
 */

#pragma once

#include <concepts>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>

#include "ClassHelper.hpp"
#include "Logger.hpp"

namespace Based {

struct Resource {
	/// Load resource to memory
	/// \param path path to resource file (if required)
	virtual void load (const std::string& path) = 0;

	/// Prepare resource for use
	/// \return If the resource is ready for use
	/// \note This stage may unload resource from memory (e.g. shaders are moved from RAM to VRAM)
	virtual bool prepare () = 0;

	/// Unload and destroy resource
	virtual void unload () = 0;

	/// Resource is loaded and may be prepared
	bool loaded = false;
	/// Resource is ready to be used
	bool ready = false;

	Resource () = default;
	virtual ~Resource () = default;
	BASED_CLASS_NO_COPY_DEFAULT_MOVE (Resource);
};

template <class T>
concept ResourceClass = std::derived_from<T, Resource>;

using ResourceMap = std::unordered_map<std::string, std::unique_ptr<Resource>>;

class ResourceManager {
	ResourceMap resourceMap;
public:
	/// Synchronously load single resource to memory
	/// \param name name of the resource
	/// \param path path to resource file (if required)
	/// \return If the resource loaded succesfully
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

	/// Synchronously prepare single loaded resource for use
	/// \param name name of the resource
	/// \return If the resource is ready for use
	bool prepare (const std::string& name) {
		if (!resourceMap.contains (name) || !resourceMap[name]->loaded) [[unlikely]] {
			log.warn ("Failed to prepare resource {}: not loaded", name);
			return false;
		}
		return resourceMap[name]->prepare();
	}

	/// Synchronously unload single resource
	/// \param name name of the resource
	/// \return If the resource unloaded succesfully
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

	/// Get pointer to loaded resource (quick, without checks)
	/// \param name name of the resource
	template <ResourceClass T>
	inline T* get (const std::string& name) {
		return static_cast<T*> (resourceMap[name].get());
	}

	ResourceManager () = default;
	BASED_CLASS_NO_COPY_MOVE (ResourceManager);
};

}
