#pragma once

#define BASED_CLASS_COPY(c, d) \
	c (const c&) = d; \
	c& operator= (const c&) = d;
#define BASED_CLASS_MOVE(c, d) \
	c (c&&) = d; \
	c& operator= (c&&) = d;
#define BASED_CLASS_NO_COPY(c) BASED_CLASS_COPY(c, delete)
#define BASED_CLASS_DEFAULT_COPY(c) BASED_CLASS_COPY(c, default)
#define BASED_CLASS_NO_MOVE(c) BASED_CLASS_MOVE(c, delete)
#define BASED_CLASS_DEFAULT_MOVE(c) BASED_CLASS_MOVE(c, default)
#define BASED_CLASS_NO_COPY_MOVE(c) \
	BASED_CLASS_NO_COPY(c); \
	BASED_CLASS_NO_MOVE(c);
#define BASED_CLASS_NO_COPY_DEFAULT_MOVE(c) \
	BASED_CLASS_NO_COPY(c); \
	BASED_CLASS_DEFAULT_MOVE(c);
