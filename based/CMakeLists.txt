add_library(based SHARED)
set_target_properties(based PROPERTIES VERSION 0.1 SOVERSION 01)
set(CMAKE_SHARED_LIBRARY_NAME_WITH_VERSION 1)

set(BASED_SOURCES
	Engine.cpp
	EngineClient.cpp
	LuaFile.cpp
	Scene.cpp
	Window.cpp
)
list(TRANSFORM BASED_SOURCES PREPEND src/)

target_sources(based PRIVATE ${BASED_SOURCES})
target_include_directories(based PUBLIC include/)

target_link_libraries(based PUBLIC 
	glad
	SDL3::SDL3
	lua::lua
	sol2::sol2
	-lstdc++exp
)
