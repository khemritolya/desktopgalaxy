cmake_minimum_required(VERSION 3.14)
project(desktopgalaxy)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(PkgConfig REQUIRED)

pkg_check_modules(X11 REQUIRED x11)
include_directories(${X11_INCLUDE_DIRS})
link_directories(${X11_LIBRARY_DIRS})

pkg_check_modules(OPENGL REQUIRED gl)
include_directories(${OPENGL_INCLUDE_DIRS})
link_directories(${OPENGL_LIBRARY_DIRS})

pkg_check_modules(GLU REQUIRED glu)
include_directories(${GLU_INCLUDE_DIRS})
link_directories(${GLU_LIBRARY_DIRS})

pkg_check_modules(GLUT REQUIRED freeglut)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS -pthread)

add_executable(desktopgalaxy main.cpp window.h window.cpp galaxy.h galaxy.cpp utility.h utility.cpp)

target_link_libraries(desktopgalaxy ${X11_LIBRARIES})
target_link_libraries(desktopgalaxy ${OPENGL_LIBRARIES})
target_link_libraries(desktopgalaxy ${GLU_LIBRARIES})