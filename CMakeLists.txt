project(HuffmanCoding)
cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++11)
add_definitions(-DVERSION="1.0")
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
add_subdirectory(libhuffmancoding)
add_subdirectory(huffman)

