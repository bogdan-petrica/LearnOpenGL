
add_executable( HelloTriangle "HelloTriangle.cpp" )
target_link_libraries( HelloTriangle glfw GLEW_static opengl32 )