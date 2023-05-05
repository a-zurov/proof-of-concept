

get_property(PROTOD GLOBAL PROPERTY PROTOBUF_DEFINED)

if(NOT PROTOD STREQUAL "YES")
    set_property(GLOBAL PROPERTY PROTOBUF_DEFINED "YES")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(Protobuf_DEBUG FALSE)
    endif()


    set(Protobuf_PREFIX_PATH "/mnt/c/github/proof-of-concept/external_sdk/protobuf/protobuf/include")
    list(APPEND CMAKE_PREFIX_PATH "${Protobuf_PREFIX_PATH}")
    find_package(Protobuf REQUIRED)

    if(Protobuf_FOUND)
        #compare version
        string(COMPARE LESS "${Protobuf_VERSION}" 3.0.0 VersionIncompatible)
        if(VersionIncompatible)
            message(FATAL_ERROR "Protobuf ${Protobuf_VERSION} not supported : only 3.0.0 or highter.")
        else(VersionIncompatible)
            message(STATUS "Found Protobuf ${Protobuf_VERSION} --${Protobuf_LIBRARIES}")
            include_directories(${Protobuf_INCLUDE_DIRS})
            link_directories(${Protobuf_LIBRARY_DIRS})
            set(REQUIRED_LIBS ${REQUIRED_LIBS} ${Protobuf_LIBRARIES})
        endif(VersionIncompatible)
    else(Protobuf_FOUND)
        message(STATUS "Protobuf NOT FOUND!")
    endif(Protobuf_FOUND)

endif()

set(REQUIRED_LIBS ${REQUIRED_LIBS} rt dl)

