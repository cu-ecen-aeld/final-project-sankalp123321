cmake_minimum_required(VERSION 3.10.2)
project(icl)

# set(tools /home/devel/gcc-4.7-linaro-rpi-gnueabihf/mnt/d/CUBoulder/ESD/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf)
set(CMAKE_C_COMPILER /mnt/d/CUBoulder/ESD/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /mnt/d/CUBoulder/ESD/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

set(CMAKE_SYSTEM_NAME Linux)
# set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_SYSROOT /mnt/d/CUBoulder/ESD/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/../arm-linux-gnueabihf/libc)
# set(CMAKE_STAGING_PREFIX /)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

add_compile_options(-g)

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
