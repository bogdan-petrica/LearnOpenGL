
add_executable( Shaders "shader.h" "shaders-main.cpp" )
target_link_libraries( Shaders glfw GLEW_static opengl32 )