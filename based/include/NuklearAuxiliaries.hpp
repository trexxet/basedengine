#pragma once

#include "Geometry.hpp"
#include "NuklearCommon.h"

const struct nk_color NK_COLOR_TRANSPARENT = {0, 0, 0, 0};

template <typename T>
struct nk_rect nk_from_Rect2D (Based::Rect2D<T> rect) {
	return nk_rect (rect.x, rect.y, rect.width, rect.height);
}
