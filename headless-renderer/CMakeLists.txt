cmake_minimum_required(VERSION 3.16)
project(EGL-Test)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
SET(GLFW FALSE)

file(GLOB SRCs
     "*.h"
     "*.cpp"
)

add_executable(main ${SRCs}) 
add_subdirectory(graphics)
add_subdirectory(src)
add_subdirectory(state)


find_package(PkgConfig) 
find_package(OpenGL REQUIRED COMPONENTS OpenGL EGL GLX)
find_package(GLEW REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIRS}  
    ${GLEW_INCLUDE_DIRS})


if(GLFW)
    find_package(glfw3 3.3 REQUIRED)
    target_link_libraries(main PRIVATE OpenGL::OpenGL OpenGL::EGL OpenGL::GLX ${GLEW_LIBRARIES} glfw)

else(GLFW)
    include(cmake-modules/FindGstreamermm.cmake)

    include_directories(
            ${GLIB_INCLUDE_DIRS}
            ${GSTREAMERMM_INCLUDE_DIRS}
    )
    link_directories(
            ${GLIB_LIBRARY_DIRS}
            ${GSTREAMERMM_INCLUDE_DIRS}
    ) 
    target_link_libraries(main PRIVATE OpenGL::OpenGL OpenGL::EGL OpenGL::GLX ${GLEW_LIBRARIES} ${GSTREAMERMM_LIBRARIES})
endif(GLFW)