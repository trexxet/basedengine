#pragma once

#include <glm/vec2.hpp>

namespace Based {

template<typename T>
using Vec2D = glm::vec<2, T>;

template <typename T>
struct Rect2D {
	union { T x, s; };
	union { T y, t; };
	T w, h;

	Rect2D (T _x, T _y, T _w, T _h) : x (_x), y (_y), w (_w), h (_h) {}
	Rect2D () : x (0), y (0), w (0), h (0) {}

	template <typename U>
	Rect2D (const Vec2D<U>& size) : x (0), y (0), w (size.x), h (size.y) {}

	template <typename U>
	Rect2D (const Vec2D<U>& pos, T _w, T _h) : x (pos.x), y (pos.y), w (_w), h (_h) {}

	template <typename U>
	Rect2D (T _x, T _y, const Vec2D<U>& size) : x (_x), y (_y), w (size.x), h (size.y) {}
	
	template <typename U>
	Rect2D (const Vec2D<T>& pos, const Vec2D<U>& size) : x (pos.x), y (pos.y), w (size.x), h (size.y) {}

	template <typename U>
	void centrify (const Rect2D<U>& outer) {
		// If (outer.width < width || outer.height < height), then
		// outer is effectively inner for that axis, so no need to check
		x = outer.x + (outer.w - w) / 2;
		y = outer.y + (outer.h - h) / 2;
	}

	template <typename U>
	operator Rect2D<U> () const {
		return { static_cast<U>(x), static_cast<U>(y), static_cast<U>(w), static_cast<U>(h) };
	}

	Vec2D<T> pos () { return Vec2D<T> {x, y}; }
	Vec2D<T> size () { return Vec2D<T> {w, h}; }
};

template <typename T>
struct Circle2D {
	union { T x, s; };
	union { T y, t; };
	T r;

	Circle2D (T _x, T _y, T _r) : x (_x), y (_y), r (_r) {}
	Circle2D () : x (0), y (0), r (0) {}

	template <typename U>
	Circle2D (const Vec2D<T>& center, const U radius) : x (center.x), y (center.y), r (radius) {}

	template <typename U>
	operator Circle2D<U> () const {
		return { static_cast<U>(x), static_cast<U>(y), static_cast<U>(r) };
	}

	Vec2D<T> pos () { return Vec2D<T> {x, y}; }
};

}
