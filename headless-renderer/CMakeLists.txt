cmake_minimum_required(VERSION 3.16)
project(EGL-Test)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS "-march=native")
set(GLFW FALSE)

file( GLOB RENDERER_SRCs graphics/*.cpp src/renderer/*.cpp)
file( GLOB RENDERER_H graphics/*.hpp src/renderer/*.hpp)

add_library( Graphics ${RENDERER_SRCs} ${RENDERER_H} )
target_link_libraries(Graphics PRIVATE ${GLEW_LIBRARIES})

file( GLOB STATE_SRCs state/*.cpp src/state/*.cpp )
file( GLOB STATE_H state/*.hpp src/state/*.hpp)

add_library( State ${STATE_SRCs} ${STATE_H} )
target_link_libraries(State PRIVATE OpenGL::OpenGL OpenGL::EGL OpenGL::GLX ${GLEW_LIBRARIES} ${GSTREAMERMM_LIBRARIES})

add_executable(state_module src/state.cpp) 
add_executable(renderer_module src/renderer.cpp) 

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
    target_link_libraries(renderer_module PRIVATE Graphics OpenGL::OpenGL OpenGL::EGL OpenGL::GLX ${GLEW_LIBRARIES} ${GSTREAMERMM_LIBRARIES})
    target_link_libraries(state_module PRIVATE State ${GLEW_LIBRARIES})
endif(GLFW)