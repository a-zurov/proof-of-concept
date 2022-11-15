project(common)
cmake_minimum_required(VERSION 2.8)

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeFiles/includes.cmake)

set(MODULE_NAME "${PROJECT_NAME}${BUILD_SUFFIX}")

set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../_compiled")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})

aux_source_directory(. SRC_LIST)
# Header without source (additional to aux_source_directory command)

set(SRC_LIST ${SRC_LIST} "./Types.h")
set(SRC_LIST ${SRC_LIST} "./AtomicSync.h")

set(SRC_LIST ${SRC_LIST} "./IBase.h")
set(SRC_LIST ${SRC_LIST} "./ILog.h")
set(SRC_LIST ${SRC_LIST} "./ILogHandler.h")
set(SRC_LIST ${SRC_LIST} "./IConnectionPointContainer.h")

set(SRC_LIST ${SRC_LIST} "./CSmartPtr.h")
set(SRC_LIST ${SRC_LIST} "./CRefCounter.h")
set(SRC_LIST ${SRC_LIST} "./CLogDispatcherImpl.h")

set(SRC_LIST ${SRC_LIST} "./SignalSlot.h")
set(SRC_LIST ${SRC_LIST} "./Singleton.h")

get_property(COMMON_D GLOBAL PROPERTY COMMON_DEFINED)

if(NOT COMMON_D STREQUAL "YES")
    set_property(GLOBAL PROPERTY COMMON_DEFINED "YES")
    add_library(${MODULE_NAME} STATIC ${SRC_LIST})
    include(${CMAKE_CURRENT_SOURCE_DIR}/../CMakeFiles/pch_support.cmake)
    add_precompiled_header(${MODULE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/pch.h)
endif()
