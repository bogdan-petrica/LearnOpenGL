
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

include_directories( "../External/glfw/include" )
include_directories( "../External/glew/include" )
add_subdirectory( framework )
add_subdirectory( hello_triangle )
add_subdirectory( hello_shaders )
add_subdirectory( hello_textures )
add_subdirectory( hello_transformations )