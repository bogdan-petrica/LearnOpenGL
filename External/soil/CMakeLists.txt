project(SOIL)
cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(SOIL_LIB_NAME SOIL)
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(SOIL_LIB_NAME SOIL)
	set(DLL_PREFIX lib)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

#
# All platforms need OpenGL
#

#include(FindPkgConfig)
#pkg_check_modules( OpenGL REQUIRED gl )
find_package( OpenGL "3.3" REQUIRED )

#
# Linux needs X11
#

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_package(X11 REQUIRED)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

set(CMAKE_C_FLAGS "${CFLAGS} ${CMAKE_C_FLAGS} -O2 -s -Wall" )

include_directories( ${PROJECT_SOURCE_DIR}/src )

add_library(SOIL_static STATIC src/SOIL.c )

set_target_properties(SOIL_static PROPERTIES OUTPUT_NAME ${SOIL_LIB_NAME} PREFIX lib)

install(
	TARGETS
		SOIL_static
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(CODE "execute_process( COMMAND bash -x -c \"sed -e 's%@prefix@%${CMAKE_INSTALL_PREFIX}%g' -e 's%@exec_prefix@%\\\${prefix}%g' -e 's%@libdir@%\\\${prefix}/lib%g' -e 's%@includedir@%\\\${prefix}/include%g' -e 's/\@version\@/${SOIL_VERSION}/g' -e 's/\@cflags\@//g' -e 's/\@libname\@/${SOILW_LIB_NAME}/g' -e 's|@requireslib@|glu|g' < ${CMAKE_SOURCE_DIR}/soil.pc.in > ${CMAKE_BINARY_DIR}/soil.pc\" )" )

install(FILES ${CMAKE_BINARY_DIR}/soil.pc DESTINATION lib/pkgconfig)
