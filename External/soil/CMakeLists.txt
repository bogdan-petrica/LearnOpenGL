project(SOIL)
cmake_minimum_required(VERSION 2.4)

include_directories( ${PROJECT_SOURCE_DIR}/src )

file(GLOB SOIL_local_SRC ${PROJECT_SOURCE_DIR}/src/*.c)
add_library(SOIL STATIC ${SOIL_local_SRC} )