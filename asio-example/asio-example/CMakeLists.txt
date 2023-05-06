cmake_minimum_required(VERSION 3.8)

project(asio-example)

include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/set_build_type.cmake)
set(MODULE_NAME "${PROJECT_NAME}${BUILD_SUFFIX}")

include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/include_boost.cmake)

set(OUTPUT_FOLDER "/../__compiled")

set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}${OUTPUT_FOLDER}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})

set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}${OUTPUT_FOLDER}")

set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})

aux_source_directory(. SRC_LIST)
# Header without source here

add_executable(${MODULE_NAME} ${SRC_LIST})

target_link_libraries( ${MODULE_NAME}
    ${Boost_LIBRARIES}
    #${REQUIRED_LIBS}
)
