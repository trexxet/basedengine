set(DEMO_TARGET 02-image_shader)
add_executable(${DEMO_TARGET}
	main.cpp
)

set(ASSET_PATH "../demo/assets/${DEMO_TARGET}")

target_compile_definitions(${DEMO_TARGET} PRIVATE
	DEMO_NAME="Demo ${DEMO_TARGET}"
	CONFIG_PATH="../demo/cfg/${DEMO_TARGET}/config.lua"
	BG_PATH="${ASSET_PATH}/crate1.png"
	SPRITE1_PATH="${ASSET_PATH}/crate2.png"
	SPRITE2_PATH="${ASSET_PATH}/crate3.png"
)
target_link_libraries(${DEMO_TARGET}
	based
)
