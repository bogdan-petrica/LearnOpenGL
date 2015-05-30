
add_executable( Engine "Engine.cpp" )
target_link_libraries( Engine glfw GLEW_static opengl32 )