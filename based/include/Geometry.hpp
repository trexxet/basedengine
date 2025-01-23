#pragma once

namespace Based {

template <typename T>
struct Point2D {
	union { T x, s, w, width; };
	union { T y, t, h, height; };

	template <typename U>
	inline bool operator== (const Point2D<U> &compar) const {
		return x == compar.x && y == compar.y;
	}

	template <typename U>
	inline void operator= (const Point2D<U> &source) {
		x = source.x;
		y = source.y;
	}

	template <typename U>
	inline void operator+= (const Point2D<U> &source) {
		x += source.x;
		y += source.y;
	}

	template <typename U>
	inline void operator-= (const Point2D<U> &source) {
		x -= source.x;
		y -= source.y;
	}

	template <typename U>
	operator Point2D<U> () const {
		return { static_cast<U>(x), static_cast<U>(y) };
	}

	Point2D (T _x, T _y) : x (_x), y (_y) {}
	Point2D () : x (0), y (0) {}
};

template <typename T>
using Size2D = Point2D<T>;

template <typename T>
struct Rect2D {
	union { T x, s; };
	union { T y, t; };
	T w, h; // Flattened Point2D + Size2D - should be a bit faster

	Rect2D (T _x, T _y, T _w, T _h) : x (_x), y (_y), w (_w), h (_h) {}
	Rect2D () : x (0), y (0), w (0), h (0) {}

	template <typename U>
	Rect2D (const Size2D<U>& size) :
		x (0), y (0), w (size.w), h (size.h) {}

	template <typename U>
	Rect2D (T _x, T _y, const Size2D<U>& size) :
		x (_x), y (_y), w (size.w), h (size.h) {}
	
	template <typename U>
	Rect2D (const Point2D<T>& pos, const Size2D<U>& size) :
		x (pos.x), y (pos.y), w (size.w), h (size.h) {}

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

	Point2D<T> pos () { return Point2D<T> {x, y}; }
	Size2D<T> size () { return Size2D<T> {w, h}; }
};

template <typename T>
struct Circle2D {
	union { T x, s; };
	union { T y, t; };
	T r; // Flattened Point2D + radius

	Circle2D (T _x, T _y, T _r) : x (_x), y (_y), r (_r) {}
	Circle2D () : x (0), y (0), r (0) {}

	template <typename U>
	Circle2D (const Point2D<T>& center, const U radius) :
		x (center.x), y (center.y), r (radius) {}

	template <typename U>
	operator Circle2D<U> () const {
		return { static_cast<U>(x), static_cast<U>(y), static_cast<U>(r) };
	}

	Point2D<T> pos () { return Point2D<T> {x, y}; }
};

}
