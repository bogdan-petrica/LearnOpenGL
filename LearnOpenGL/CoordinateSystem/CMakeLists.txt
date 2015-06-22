add_executable( CoordinateSystem "coordSys.cpp" )
target_link_libraries( CoordinateSystem Common glfw GLEW_static opengl32 SOIL )