# CMakeList.txt : CMake project for NetCommon, include source and define project specific logic here.
# ---------------------------------------------------------------------------------------------------
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("NetCommon")

include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/set_build_type.cmake)
set(MODULE_NAME "${PROJECT_NAME}${BUILD_SUFFIX}")

include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/include_boost.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/include_protobuf.cmake)

file(GLOB PROTO_LIST "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
foreach(PROTO_FILE ${PROTO_LIST})
    if(EXISTS ${PROTO_FILE})
        MESSAGE(STATUS "Protoc assined:\n   ${PROTO_FILE}")
    endif()
endforeach()

protobuf_generate_cpp(
    PROTO_SRCS
    PROTO_HDRS
    ${PROTO_LIST}
)
message(STATUS "Protoc generated:\n   ${PROTO_SRCS}")

aux_source_directory(. SRC_LIST)
# Header without source here (additional to aux_source_directory command)
set(SRC_LIST ${SRC_LIST} "./net_client.h")
set(SRC_LIST ${SRC_LIST} "./net_common.h")
set(SRC_LIST ${SRC_LIST} "./net_connection.h")
set(SRC_LIST ${SRC_LIST} "./net_message.h")
set(SRC_LIST ${SRC_LIST} "./net_server.h")
set(SRC_LIST ${SRC_LIST} "./net_tsqueue.h")
set(SRC_LIST ${SRC_LIST} "./olc_net.h")

get_property(ncdef GLOBAL PROPERTY NetCommon_DEFINED)
if(NOT ncdef STREQUAL "YES")
    set_property(GLOBAL PROPERTY NetCommon_DEFINED "YES")
    add_library(${MODULE_NAME} STATIC ${PROTO_SRCS} ${SRC_LIST})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${MODULE_NAME} PROPERTY CXX_STANDARD 20)
  # TODO: Add tests and install targets if needed..
else()
  set_property(TARGET ${MODULE_NAME} PROPERTY CXX_STANDARD 17)
endif()
