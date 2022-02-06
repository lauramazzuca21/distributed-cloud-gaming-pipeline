file(GLOB SRCs
     "*.h"
     "*.cpp"
)

target_sources(main
        PRIVATE
        ${SRCs}
        )