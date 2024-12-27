cmake_minimum_required(VERSION 3.30)
project(based_engine VERSION 0.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_CXX_STANDARD 23)

include(cmake/getSDL.cmake)
include(cmake/getLua.cmake)

add_subdirectory(based)
add_subdirectory(demo)
