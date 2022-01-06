cmake_minimum_required(VERSION 3.16)
project(EGL-Test)
file(GLOB SRCs
     "*.h"
     "*.cpp"
)

add_executable(main ${SRCs}) 
find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL GLX)
# find_package(GLUT REQUIRED) # ${GLUT_INCLUDE_DIRS} # ${GLUT_LIBRARY}
find_package(GLEW REQUIRED)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLEW_INCLUDE_DIRS})
target_link_libraries(main PRIVATE OpenGL::OpenGL OpenGL::EGL OpenGL::GLX ${GLEW_LIBRARIES} ${OpenCV_LIBS})