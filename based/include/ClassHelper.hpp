#pragma once

#define BASED_CLASS_COPY(c, d) \
	c (const c&) = d; \
	c& operator= (const c&) = d;
#define BASED_CLASS_MOVE(c, d) \
	c (c&&) = d; \
	c& operator= (c&&) = d;
