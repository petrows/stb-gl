cmake_minimum_required (VERSION 2.6)
 
set(PROJECT "opengl-hal")
project(${PROJECT})

file (GLOB_RECURSE HAL_SRC "src/opengl-stb/*.c*")
file (GLOB_RECURSE HAL_H "src/opengl-stb/*.h")

file (GLOB_RECURSE APP_SRC "src/app/*.cpp")
file (GLOB_RECURSE APP_H "src/app/*.h")

# apt-get install libxmu-dev libxi-dev freeglut3 freeglut3-dev libsoil-dev libpng12-dev

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)

find_library( SOIL_LIBRARIES SOIL )

include_directories("src/")
include_directories( ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )

add_executable ( ${PROJECT} WIN32 ${APP_SRC} ${APP_H} ${HAL_SRC} ${HAL_H} )
target_link_libraries( ${PROJECT} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARIES} ${SOIL_LIBRARIES})

file (GLOB_RECURSE APP_RES "res/*")
install(FILES ${APP_RES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
