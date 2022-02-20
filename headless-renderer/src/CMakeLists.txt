file(GLOB SRCs
     "*.h"
     "*.cpp"
)
SET(GLFW TRUE)

if(GLFW)
    list(REMOVE_ITEM SRCs "${CMAKE_CURRENT_LIST_DIR}/GstRawFramesApp.cpp")
    list(REMOVE_ITEM SRCs "${CMAKE_CURRENT_LIST_DIR}/GstRawFramesApp.h")
else(GLFW)
    list(REMOVE_ITEM SRCs "${CMAKE_CURRENT_LIST_DIR}/RawFramesApp.cpp")
    list(REMOVE_ITEM SRCs "${CMAKE_CURRENT_LIST_DIR}/RawFramesApp.h")
endif(GLFW)

target_sources(main
        PRIVATE
        ${SRCs}
        )