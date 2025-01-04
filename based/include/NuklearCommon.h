#pragma once

#include <glad/gl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#include <nuklear.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <nuklear_sdl_gl3.h>
#ifdef __cplusplus
}
#endif

#define NK_MAX_VERTEX_MEMORY 512 * 1024
#define NK_MAX_ELEMENT_MEMORY 128 * 1024
