#pragma once

namespace Based {

template <typename T>
struct Point2D {
	union { T x, width; };
	union { T y, height; };

	template<typename U>
	inline bool operator== (const Point2D<U> &compar) const {
		return x == compar.x && y == compar.y;
	}

	template<typename U>
	inline void operator= (const Point2D<U> &source) {
		x = source.x;
		y = source.y;
	}

	template<typename U>
	inline void operator+= (const Point2D<U> &source) {
		x += source.x;
		y += source.y;
	}

	template<typename U>
	inline void operator-= (const Point2D<U> &source) {
		x -= source.x;
		y -= source.y;
	}

	Point2D (T _x, T _y) : x(_x), y(_y) {}
};

template <typename T>
using Size2D = Point2D<T>;

}
