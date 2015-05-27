include_directories( "." )
include_directories( "../framework/src" )

file(GLOB local_SRC "*.cpp")
add_executable( HelloTextures ${local_SRC} )

target_link_libraries( HelloTextures glfw GLEW_static opengl32 DSGLF)