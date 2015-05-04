include_directories( "." )
add_executable( HelloShaders "HelloShaders.cpp" )
target_link_libraries( HelloShaders glfw GLEW_static opengl32 )