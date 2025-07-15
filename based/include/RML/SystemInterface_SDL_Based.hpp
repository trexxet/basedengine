/* A wrapper for SystemInterface_SDL which routes logging through Based Logger */
#pragma once

#include <string_view>

#include <RmlUi_Platform_SDL.h>

#include "Logger.hpp"

namespace Based::RML {

class SystemInterface_SDL_Based : public SystemInterface_SDL {
	bool LogMessage(Rml::Log::Type type, const Rml::String& message) override {
		log.write ("[RML]{} {}",
			[type]() constexpr -> std::string_view {
				switch (type) {
					case Rml::Log::Type::LT_ERROR:   return "[ERROR]";
					case Rml::Log::Type::LT_WARNING: return "[WARN]";
					default: return "";
				}
			} (),
			message);
		return true;
	}
};

}
