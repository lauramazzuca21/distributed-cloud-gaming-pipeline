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
find_package(PkgConfig) 
pkg_search_module(GLIB REQUIRED glib-2.0) 
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GST_APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLEW_INCLUDE_DIRS})
target_link_libraries(main PRIVATE OpenGL::OpenGL OpenGL::EGL OpenGL::GLX ${GLEW_LIBRARIES} ${OpenCV_LIBS} ${GST_APP_LIBRARIES} ${GST_VIDEO_LIBRARIES})