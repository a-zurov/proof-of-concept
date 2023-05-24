cmake_minimum_required(VERSION 3.8)

project(client)

set(MODULE_NAME "${PROJECT_NAME}${BUILD_SUFFIX}")

#set(SRC_LIST "udp-client-zero.cpp")
set(SRC_LIST "udp-client-nonblock.cpp")

add_executable(${MODULE_NAME} ${SRC_LIST})
