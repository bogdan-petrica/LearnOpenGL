
add_executable( Texure "textures_combined.cpp" "../Shaders/shader.h" )
target_link_libraries( Texure glfw GLEW_static opengl32 SOIL )