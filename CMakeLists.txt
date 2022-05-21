cmake_minimum_required(VERSION 3.22)
project(Clipping_program)

set(CMAKE_CXX_STANDARD 14)

add_executable(Clipping_program main.cpp src/libs.h Window.cpp Window.h)

add_subdirectory(ImGui)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLFW_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/ImGui/inc REQUIRED)
link_directories(${PROJECT_SOURCE_DIR}/ImGui/src REQUIRED)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLFW3 REQUIRED)

target_link_libraries(${PROJECT_NAME} ImGui ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLFW_LIBRARY} mingw32 opengl32 glfw3 freeglut glu32 glew32)

set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")