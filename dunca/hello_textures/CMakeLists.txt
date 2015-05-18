include_directories( "." )

file(GLOB framework_SRC "../framework/*.cpp")
file(GLOB local_SRC "*.cpp")

add_executable( HelloTextures ${local_SRC} ${framework_SRC} )

target_link_libraries( HelloTextures glfw GLEW_static opengl32 SOIL_static )