set(LUA_PREFIX_SRC ${LUA_PREFIX_PATH}/src/)
set(LUA_PREFIX_INCLUDE ${LUA_PREFIX_PATH}/include/)

set(LUA_LIB_SRCS 
	lapi.c
	lcode.c
	lctype.c
	ldebug.c
	ldo.c
	ldump.c
	lfunc.c
	lgc.c
	llex.c
	lmem.c
	lobject.c
	lopcodes.c
	lparser.c
	lstate.c
	lstring.c
	ltable.c
	ltm.c
	lundump.c
	lvm.c
	lzio.c
	lauxlib.c
	lbaselib.c
	lcorolib.c
	ldblib.c
	liolib.c
	lmathlib.c
	loadlib.c
	loslib.c
	lstrlib.c
	ltablib.c
	lutf8lib.c
	linit.c
)
list(TRANSFORM LUA_LIB_SRCS PREPEND ${LUA_PREFIX_SRC})

# copy includes to include/
set(LUA_LIB_INCLUDES
	lauxlib.h
	lua.h
	lua.hpp
	luaconf.h
	lualib.h
)
file(MAKE_DIRECTORY ${LUA_PREFIX_INCLUDE})
foreach(LUA_LIB_INCLUDE IN LISTS LUA_LIB_INCLUDES)
	file(COPY_FILE
	${LUA_PREFIX_SRC}${LUA_LIB_INCLUDE}
	${LUA_PREFIX_INCLUDE}${LUA_LIB_INCLUDE})
endforeach()

add_library(lua SHARED ${LUA_LIB_SRCS})
if(WIN32)
	target_compile_definitions(lua PRIVATE LUA_BUILD_AS_DLL)
endif()
target_include_directories(lua INTERFACE ${LUA_PREFIX_INCLUDE})
set_target_properties(lua PROPERTIES VERSION 5.4.7 SOVERSION 547)
set(CMAKE_SHARED_LIBRARY_NAME_WITH_VERSION 1)
add_library(lua::lua ALIAS lua)

