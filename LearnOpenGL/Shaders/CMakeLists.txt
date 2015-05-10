
add_executable( Shaders "Shaders.cpp" "GLProgram.h" "GLProgram.cpp" )
target_link_libraries( Shaders glfw GLEW_static opengl32 )