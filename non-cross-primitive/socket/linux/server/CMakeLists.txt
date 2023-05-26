cmake_minimum_required(VERSION 3.8)

project(server)

set(MODULE_NAME "${PROJECT_NAME}${BUILD_SUFFIX}")

#set(SRC_LIST "udp-server-zero.cpp")
#set(SRC_LIST "udp-server-nonblock.cpp")
#set(SRC_LIST "tcp-server-zero.cpp")
#set(SRC_LIST "tcp-server-select.cpp")
set(SRC_LIST "tcp-server-poll.cpp")

add_executable(${MODULE_NAME} ${SRC_LIST})
