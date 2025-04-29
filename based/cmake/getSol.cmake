include(FetchContent)

set(SOL2_BUILD_LUA FALSE CACHE BOOL "Always build Lua, do not search for it in the system")
option(SOL2_ENABLE_INSTALL "Enable installation of Sol2" OFF)

FetchContent_Declare(
	sol
	GIT_REPOSITORY https://github.com/ThePhD/sol2.git
	GIT_TAG        v3.5.0
	GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(sol)
