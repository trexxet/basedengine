include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm
	GIT_TAG        1.0.1
	GIT_SHALLOW    TRUE
)

option(GLM_ENABLE_CXX_20 "Enable C++ 20" ON)

FetchContent_MakeAvailable(glm)
