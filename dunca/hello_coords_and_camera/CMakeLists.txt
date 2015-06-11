include_directories( "." )
include_directories( "../../External/glm" )
include_directories( "../framework/src" )

file(GLOB local_SRC "*.cpp")
add_executable( HelloCoordsAndCamera ${local_SRC} )

target_link_libraries( HelloCoordsAndCamera glfw GLEW_static opengl32 DSGLF )