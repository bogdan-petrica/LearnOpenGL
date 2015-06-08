add_executable( Transformations "transformation.cpp" )
target_link_libraries( Transformations Common glfw GLEW_static opengl32 SOIL )