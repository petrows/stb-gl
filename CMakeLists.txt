cmake_minimum_required (VERSION 2.8)
 
set(PROJECT "stb-gl")
project(${PROJECT})

file (GLOB_RECURSE HAL_SRC "src/stbgl/*.c*")
file (GLOB_RECURSE HAL_H "src/stbgl/*.h")

file (GLOB_RECURSE APP_SRC "src/app/*.cpp")
file (GLOB_RECURSE APP_H "src/app/*.h")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(Freetype REQUIRED)

include_directories("src/")
include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})

add_definitions (${PNG_DEFINITIONS} -DPNG_SKIP_SETJMP_CHECK)
add_executable ( ${PROJECT} WIN32 ${APP_SRC} ${APP_H} ${HAL_SRC} ${HAL_H} )
target_link_libraries( ${PROJECT} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARIES} ${FREETYPE_LIBRARIES} pthread)

file (GLOB_RECURSE APP_RES "res/*")
install(FILES ${APP_RES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
