include(FetchContent)

FetchContent_Declare(
	glad
	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
	GIT_TAG        v2.0.8
	GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(glad)

if(glad_POPULATED)
	add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
	glad_add_library(glad STATIC REPRODUCIBLE EXCLUDE_FROM_ALL API gl:core=3.3 EXTENSIONS NONE)
endif()
