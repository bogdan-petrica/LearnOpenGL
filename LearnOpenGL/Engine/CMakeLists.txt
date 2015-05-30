
add_executable( Engine "Engine.cpp" "Shader.h" )
target_link_libraries( Engine glfw GLEW_static opengl32 )