# CMakeList.txt : CMake project for protobuf-example, include source and define project specific logic here.
# ----------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("protobuf-example")

include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/set_build_type.cmake)
set(MODULE_NAME "${PROJECT_NAME}${BUILD_SUFFIX}")

include(${CMAKE_CURRENT_SOURCE_DIR}/../_cmake_files/include_protobuf.cmake)

# protoc -I=. --cpp_out=. ./addressbook.proto
protobuf_generate_cpp(
    PROTO_SRCS
    PROTO_HDRS
    addressbook.proto
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable( ${MODULE_NAME}
  protobuf-example.cpp
  ${PROTO_SRCS}
  ${PROTO_HDRS}
)

target_link_libraries( ${MODULE_NAME}
    ${Protobuf_LIBRARIES}
)

set(CMAKE_CXX_STANDARD 17)
