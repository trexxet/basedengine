set(DEMO_TARGET 01-helloworld)
add_executable(${DEMO_TARGET}
	main.cpp
)
target_compile_definitions(${DEMO_TARGET} PRIVATE
	DEMO_NAME="Demo ${DEMO_TARGET}"
	CONFIG_PATH="../demo/cfg/${DEMO_TARGET}/config.lua"
	ENABLE_CLIENT=true
)
target_link_libraries(${DEMO_TARGET}
	based
)
