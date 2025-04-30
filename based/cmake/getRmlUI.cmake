include(FetchContent)

option(RMLUI_SAMPLES "Build samples of the library." OFF)
set(RMLUI_FONT_ENGINE "freetype" CACHE STRING "Font engine to build RmlUi with. If set to \"none\", RmlUi won't build with any of the included font engines and some samples might be disabled.")

FetchContent_Declare(
	RmlUi
	GIT_REPOSITORY https://github.com/mikke89/RmlUi.git
	GIT_TAG        6.1
	GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(RmlUi)

set(RMLUI_BACKEND_PATH ${RmlUi_SOURCE_DIR}/Backends)
add_library(rmlui_backend_SDL_GL3 INTERFACE)
target_sources(rmlui_backend_SDL_GL3 INTERFACE
	${RMLUI_BACKEND_PATH}/RmlUi_Platform_SDL.cpp
	${RMLUI_BACKEND_PATH}/RmlUi_Renderer_GL3.cpp
)
target_include_directories(rmlui_backend_SDL_GL3 INTERFACE ${RMLUI_BACKEND_PATH})
target_compile_definitions(rmlui_backend_SDL_GL3 INTERFACE
	RMLUI_SDL_VERSION_MAJOR=3
	RMLUI_GL3_CUSTOM_LOADER=<glad/gl.h>
)
