file(GLOB SRCs
     "*.h"
     "*.cpp"
)

# SET(GLFW TRUE)

# if(GLFW)
#     list(REMOVE_ITEM SRCs "${CMAKE_CURRENT_LIST_DIR}/ContextEGL.cpp")
#     list(REMOVE_ITEM SRCs "${CMAKE_CURRENT_LIST_DIR}/ContextEGL.h")
# endif(GLFW)

target_sources(main
        PRIVATE
        ${SRCs}
        )