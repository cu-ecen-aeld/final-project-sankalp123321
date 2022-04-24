cmake_minimum_required(VERSION 3.10.2)
project(icl)

set(CMAKE_TOOLCHAIN_FILE)
add_compile_options(-g -Wall -Werror)
add_library(icl STATIC
    ${CMAKE_SOURCE_DIR}/../../src/main.cpp
    ${CMAKE_SOURCE_DIR}/../../src/logger/logger.cpp
    ${CMAKE_SOURCE_DIR}/../../src/buffer/buffer.cpp
    ${CMAKE_SOURCE_DIR}/../../src/packet/packet.cpp
    ${CMAKE_SOURCE_DIR}/../../src/ackMsg/ackMsg.cpp
    ${CMAKE_SOURCE_DIR}/../../src/socket/socket.cpp
    ${CMAKE_SOURCE_DIR}/../../src/router/routingTbl.cpp
    ${CMAKE_SOURCE_DIR}/../../src/threadMgmt/threadMgmt.cpp
    ${CMAKE_SOURCE_DIR}/../../src/threadBase/threadBase.cpp
    ${CMAKE_SOURCE_DIR}/../../src/threadClient/threadClient.cpp
    ${CMAKE_SOURCE_DIR}/../../src/threadServer/threadServer.cpp
)

target_include_directories(icl PRIVATE
    -lpthread
    ${CMAKE_SOURCE_DIR}/../../includes/socket
    ${CMAKE_SOURCE_DIR}/../../includes/router
    ${CMAKE_SOURCE_DIR}/../../includes/logger
    ${CMAKE_SOURCE_DIR}/../../includes/threadMgmt
    ${CMAKE_SOURCE_DIR}/../../includes
    ${CMAKE_SOURCE_DIR}/../../includes/buffer
    ${CMAKE_SOURCE_DIR}/../../includes/packet
    ${CMAKE_SOURCE_DIR}/../../includes/ackMsg
    ${CMAKE_SOURCE_DIR}/../../includes/threadBase
    ${CMAKE_SOURCE_DIR}/../../includes/threadClient
    ${CMAKE_SOURCE_DIR}/../../includes/threadServer
 )