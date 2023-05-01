cmake_minimum_required(VERSION 3.10.0)

project(NetServer)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/include_boost.cmake)

set(MODULE_NAME "${PROJECT_NAME}${BUILD_SUFFIX}")

set(OUTPUT_FOLDER "/../__compiled")

set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}${OUTPUT_FOLDER}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})

set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}${OUTPUT_FOLDER}")

set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})

get_property(NCD GLOBAL PROPERTY NETCOMMON_DEFINED)

if(NOT NCD STREQUAL "YES")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../NetCommon" "${CMAKE_CURRENT_SOURCE_DIR}/../NetCommon/${BUILD_MODIFIER}")
endif()

aux_source_directory(. SRC_LIST)
# Header without source here
set(SRC_LIST ${SRC_LIST} "./../NetCommon/olc_net.h")

add_executable(${MODULE_NAME} ${SRC_LIST})

link_directories(${CMAKE_CURRENT_SOURCE_DIR}${OUTPUT_FOLDER})

target_link_libraries(
    ${MODULE_NAME}
    "NetCommon${BUILD_SUFFIX}"
    ${REQUIRED_LIBS}
)
