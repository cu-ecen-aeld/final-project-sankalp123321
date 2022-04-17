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

add_executable(client 
    ${CMAKE_SOURCE_DIR}/../../src/main.cpp
)

target_link_libraries(client
    -pthread
    ${CMAKE_SOURCE_DIR}/libicl.a
)

target_include_directories(client PRIVATE
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
