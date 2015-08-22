aux_source_directory(. SRC_LIST)
add_library(LIBHUFFMANCODING ${SRC_LIST} ${HEADERS})
install( FILES ${HEADERS} DESTINATION include)
include_directories("${PROJECT_BINARY_DIR}")
