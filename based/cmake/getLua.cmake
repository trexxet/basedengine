include(FetchContent)

set(LUA_VERSION "lua-5.4.7")

FetchContent_Declare(
	lua_src
	URL https://www.lua.org/ftp/${LUA_VERSION}.tar.gz
	URL_HASH SHA256=9fbf5e28ef86c69858f6d3d34eccc32e911c1a28b4120ff3e84aaa70cfbf1e30
)
FetchContent_MakeAvailable(lua_src)
FetchContent_GetProperties(lua_src)

set(LUA_PREFIX_PATH ${lua_src_SOURCE_DIR})
include(cmake/buildLua.cmake)
