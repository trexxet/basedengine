#pragma once

#include <string>

#include <glad/gl.h>

#include "Geometry.hpp"
#include "Logger.hpp"
#include "LuaFile.hpp"

struct Config {
	Based::Size2D<int> windowSize;

	struct {
		std::string textureBg, texture1, texture2, texture3;
	} path;

	GLfloat hex1R;
	Based::Rect2D<GLfloat> sprite3_rect;

	void load (const std::string& configPath) {
		Based::Lua::File conf (configPath, Based::Lua::BindTypes::Geometry);

		windowSize = conf["window"];
		path.textureBg = conf["path"]["textureBg"];
		path.texture1 = conf["path"]["texture1"];
		path.texture2 = conf["path"]["texture2"];
		path.texture3 = conf["path"]["texture3"];
		hex1R = conf["hex1_outerRadius"];
		sprite3_rect = conf["sprite3_rect"];

		Based::log.write ("Loaded config: {}", configPath);
	}
};
