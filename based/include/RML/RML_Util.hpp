#pragma once

#include <RmlUi/Core/Math.h>

#include "Geometry.hpp"

namespace Based::RML {

template <typename T>
class Rml::Vector2<T> to_Vec2 (const Vec2D<T>& vec) {
	return {vec.x, vec.y};
}

}
