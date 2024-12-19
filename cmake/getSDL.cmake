if(NOT SDL3_FOUND)
	include(FetchContent)
	set(SDL_SHARED TRUE CACHE BOOL "Build SDL shared library (if available)")
	set(SDL_STATIC FALSE CACHE BOOL "Build SDL static library (if available)")
	FetchContent_Declare(
		SDL
		GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
		GIT_TAG main
		GIT_SHALLOW TRUE
		GIT_PROGRESS TRUE
	)
	FetchContent_MakeAvailable(SDL)
	set_property(DIRECTORY "${PROJECT_BINARY_DIR}/_deps/sdl-src" PROPERTY EXCLUDE_FROM_ALL TRUE)
endif()
