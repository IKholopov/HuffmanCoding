aux_source_directory(. SRC_LIST)
add_library(HuffmanCoding SHARED ${SRC_LIST} ${HEADERS})
install( FILES ${HEADERS} DESTINATION include)
include_directories("${PROJECT_BINARY_DIR}")
INSTALL(TARGETS HuffmanCoding DESTINATION "lib")
INSTALL(FILES huffmancoding.h
        DESTINATION "../include"
        )
