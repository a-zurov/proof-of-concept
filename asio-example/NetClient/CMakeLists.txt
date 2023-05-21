# CMakeList.txt : CMake project for NetClient, include source and define project specific logic here.
# ---------------------------------------------------------------------------------------------------
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("NetClient")

include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/set_build_type.cmake)
set(MODULE_NAME "${PROJECT_NAME}${BUILD_SUFFIX}")

include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/include_boost.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/include_protobuf.cmake)

get_property(ncdef GLOBAL PROPERTY NetCommon_DEFINED)
if(NOT ncdef STREQUAL "YES")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../NetCommon" "${CMAKE_CURRENT_SOURCE_DIR}/../NetCommon/out/build/linux-${BUILD_MODIFIER}")
 endif()

 get_property(netlogdef GLOBAL PROPERTY NetLog_DEFINED)
if(NOT netlogdef STREQUAL "YES")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../NetLog" "${CMAKE_CURRENT_SOURCE_DIR}/../NetLog/out/build/linux-${BUILD_MODIFIER}")
 endif()

aux_source_directory(. SRC_LIST)
# Header without source here
set(SRC_LIST ${SRC_LIST} "${CMAKE_CURRENT_SOURCE_DIR}/../NetCommon/olc_net.h")

add_executable(${MODULE_NAME} ${PROTO_HDRS} ${SRC_LIST})

link_directories(${CMAKE_CURRENT_SOURCE_DIR}${OUTPUT_FOLDER})

target_link_libraries( ${MODULE_NAME}
    "NetCommon${BUILD_SUFFIX}"
    "NetLog${BUILD_SUFFIX}"
    ${Boost_LIBRARIES}
    ${Protobuf_LIBRARIES}
)

file(GLOB PROTO_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../NetCommon/out/build/linux-${BUILD_MODIFIER}/*.pb.*")
foreach(PROTO_FILE ${PROTO_LIST})
    add_custom_command( TARGET ${MODULE_NAME} PRE_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy ${PROTO_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/../NetCommon
                      )
    message(STATUS "Copy assined:\n   ${PROTO_FILE}")
endforeach()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${MODULE_NAME} PROPERTY CXX_STANDARD 20)
  # TODO: Add tests and install targets if needed..
else()
  set_property(TARGET ${MODULE_NAME} PROPERTY CXX_STANDARD 17)
endif()
