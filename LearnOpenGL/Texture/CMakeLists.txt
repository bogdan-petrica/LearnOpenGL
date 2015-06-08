
add_executable( Texure "../Shaders/shader.h" "textures_combined.cpp" )
target_link_libraries( Texure glfw GLEW_static opengl32 SOIL )