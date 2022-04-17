cmake_minimum_required(VERSION 3.10.2)
project(scooby)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-g)

add_executable(scooby 
    ${CMAKE_SOURCE_DIR}/../../src/main.cpp
)

target_link_libraries(scooby
    -pthread
    ${CMAKE_SOURCE_DIR}/libicl.a
)

target_include_directories(scooby PRIVATE
    -lpthread
    ${CMAKE_SOURCE_DIR}/../../includes/socket
    ${CMAKE_SOURCE_DIR}/../../includes/router
    ${CMAKE_SOURCE_DIR}/../../includes/logger
    ${CMAKE_SOURCE_DIR}/../../includes/threadMgmt
    ${CMAKE_SOURCE_DIR}/../../includes
    ${CMAKE_SOURCE_DIR}/../../includes/buffer
    ${CMAKE_SOURCE_DIR}/../../includes/packet
    ${CMAKE_SOURCE_DIR}/../../includes/threadBase
    ${CMAKE_SOURCE_DIR}/../../includes/threadClient
    ${CMAKE_SOURCE_DIR}/../../includes/threadServer
 )