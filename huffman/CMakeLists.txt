include_directories("${CMAKE_SOURCE_DIR}/libhuffmancoding")
aux_source_directory(. SRC_LIST)
add_executable(huffman ${SRC_LIST})
target_link_libraries(huffman HuffmanCoding)
include_directories("${PROJECT_BINARY_DIR}")
install(TARGETS huffman DESTINATION bin)

