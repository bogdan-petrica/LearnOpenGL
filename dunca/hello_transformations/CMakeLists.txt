include_directories( "." )
include_directories( "../../External/glm/glm" )
include_directories( "../framework/src" )

file(GLOB local_SRC "*.cpp")
add_executable( HelloTransformations ${local_SRC} )

target_link_libraries( HelloTransformations glfw GLEW_static opengl32 DSGLF )