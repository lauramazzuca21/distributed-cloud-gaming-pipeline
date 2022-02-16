cmake_minimum_required(VERSION 3.16)
project(MLS)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB SRCs
     "*.h"
     "*.c"
)

add_executable(main ${SRCs}) 


find_package(PkgConfig) 
include(cmake-modules/FindGstreamer.cmake)
include(cmake-modules/FindGlib.cmake)
include(cmake-modules/FindJsonGlib.cmake)
include(cmake-modules/FindLibSoup2.cmake)

include_directories(
        ${GSTREAMER_INCLUDE_DIR}
        ${GSTREAMER_WEBRTC_INCLUDE_DIR}
        ${JsonGlib_INCLUDE_DIRS}
        ${Glib_INCLUDE_DIRS}
        ${LibSoup2_INCLUDE_DIRS}

)
link_directories(
        ${GSTREAMER_WEBRTC_LIBRARY}
        ${GSTREAMER_LIBRARIES}
        ${GSTREAMER_SDP_LIBRARY}
        ${JsonGlib_LIBRARIES}
        ${Glib_LIBRARIES}
        ${LibSoup2_LIBRARIES}
) 

target_link_libraries(main PRIVATE ${GSTREAMER_LIBRARIES} ${GSTREAMER_WEBRTC_LIBRARY} ${GSTREAMER_SDP_LIBRARY} ${JsonGlib_LIBRARIES} ${Glib_LIBRARIES} ${LibSoup2_LIBRARIES})