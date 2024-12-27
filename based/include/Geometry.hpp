#pragma once

namespace Based {

template <typename T>
struct Point2D {
	T x, y;

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
};

template <typename T>
struct Size2D {
	T width, height;
};

}
