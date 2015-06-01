
add_executable( Engine "Engine.cpp" "Shader.h" "Stuff.h" "Stuff.cpp" )
target_link_libraries( Engine glfw GLEW_static opengl32 SOIL )