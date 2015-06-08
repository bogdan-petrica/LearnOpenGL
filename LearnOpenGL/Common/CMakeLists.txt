

add_library( Common "GLProgram.cpp" "GLProgram.h" "VertexArrayObject.h" "Uniform.h" )
target_link_libraries( Common glfw GLEW_static opengl32 )