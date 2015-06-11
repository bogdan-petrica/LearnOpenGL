project(DSGLF)
cmake_minimum_required(VERSION 2.4)

include_directories( ${PROJECT_SOURCE_DIR}/src )
include_directories( "../../External/soil/src" )
include_directories( "../../External/glm" )

file(GLOB DSGLF_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(DSGLF STATIC ${DSGLF_SRC} )

target_link_libraries(DSGLF glfw GLEW_static opengl32 SOIL)