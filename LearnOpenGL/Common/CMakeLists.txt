

add_library( Common "GLProgram.cpp" "GLProgram.h" "VertexArrayObject.h" )
target_link_libraries( Common glfw GLEW_static opengl32 )