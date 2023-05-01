cmake_minimum_required(VERSION 3.10.0)

project(NetCommon)

include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/include_boost.cmake)

set(MODULE_NAME "${PROJECT_NAME}${BUILD_SUFFIX}")

set(OUTPUT_FOLDER "/../__compiled")

set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}${OUTPUT_FOLDER}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})

aux_source_directory(. SRC_LIST)
# Header without source (additional to aux_source_directory command)

set(SRC_LIST ${SRC_LIST} "./net_client.h")
set(SRC_LIST ${SRC_LIST} "./net_common.h")
set(SRC_LIST ${SRC_LIST} "./net_connection.h")
set(SRC_LIST ${SRC_LIST} "./net_message.h")
set(SRC_LIST ${SRC_LIST} "./net_server.h")
set(SRC_LIST ${SRC_LIST} "./net_tsqueue.h")
set(SRC_LIST ${SRC_LIST} "./olc_net.h")

get_property(NCD GLOBAL PROPERTY NETCOMMON_DEFINED)

if(NOT NCD STREQUAL "YES")
    set_property(GLOBAL PROPERTY NETCOMMON_DEFINED "YES")
    add_library(${MODULE_NAME} STATIC ${SRC_LIST})
    #include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeFiles/pch_support.cmake)
    #add_precompiled_header(${MODULE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/pch.h)
endif()
