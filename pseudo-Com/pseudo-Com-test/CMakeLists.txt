project(pseudo-com-test)
cmake_minimum_required(VERSION 2.8)

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeFiles/includes.cmake)

set(MODULE_NAME "${PROJECT_NAME}${BUILD_SUFFIX}")

set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../_compiled")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})

set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../_compiled")

set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})

get_property(COMMON_D GLOBAL PROPERTY COMMON_DEFINED)

if(NOT COMMON_D STREQUAL "YES")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Common" "${CMAKE_CURRENT_SOURCE_DIR}/../Common/${BUILD_MODIFIER}")
endif()

aux_source_directory(. SRC_LIST)
# Header without source (additional to aux_source_directory command)

add_executable(${MODULE_NAME} ${SRC_LIST})

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../_compiled)

target_link_libraries(
    ${MODULE_NAME}
    "common${BUILD_SUFFIX}"
    ${REQUIRED_LIBS}
)
