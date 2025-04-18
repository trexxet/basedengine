add_library(based SHARED)

include(cmake/getGlad.cmake)
include(cmake/getGlm.cmake)
include(cmake/getSDL.cmake)
include(cmake/getSDL_image.cmake)
include(cmake/getLua.cmake)
include(cmake/getSol.cmake)
include(cmake/getNuklear.cmake)

target_compile_definitions(based PUBLIC
	BASED_VERSION="0.1-dev-alpha"
)
target_compile_definitions(based PRIVATE
	$<$<CONFIG:Debug>:BASED_ENABLE_GL_CHECK>
)
set_target_properties(based PROPERTIES
	VERSION 0.1
	SOVERSION 01
	DEBUG_POSTFIX "dbg"
)
set(CMAKE_SHARED_LIBRARY_NAME_WITH_VERSION 1)

set(BASED_SOURCES
	Engine.cpp
	EngineClient.cpp
	LuaFile.cpp
	NuklearImplement.c
	Window.cpp
	GL/GL_Util.cpp
	GL/Polygon.cpp
	GL/Shader.cpp
	GL/Sprite.cpp
	GL/Texture.cpp
)
list(TRANSFORM BASED_SOURCES PREPEND src/)

target_sources(based PRIVATE ${BASED_SOURCES})
target_include_directories(based PUBLIC include/)
target_include_directories(based PRIVATE include_internal/)

target_link_libraries(based PUBLIC 
	glad
	glm::glm
	SDL3::SDL3
	SDL3_image::SDL3_image
	lua::lua
	sol2::sol2
	nuklear::nk
	-lstdc++exp
)
