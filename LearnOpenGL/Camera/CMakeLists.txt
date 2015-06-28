add_executable( Camera "camera.cpp" "ICamera.h" "FPSCamera.h" )
target_link_libraries( Camera Common glfw GLEW_static opengl32 SOIL )