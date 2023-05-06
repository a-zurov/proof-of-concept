
get_property(proto_def GLOBAL PROPERTY PROTOBUF_DEFINED)

if(NOT proto_def STREQUAL "YES")

    set_property(GLOBAL PROPERTY PROTOBUF_DEFINED "YES")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(Protobuf_DEBUG FALSE)
    endif()

    find_package(Protobuf REQUIRED)

    if(Protobuf_FOUND)
        #compare version
        string(COMPARE LESS "${Protobuf_VERSION}" 3.0.0 version_incompatible)
        if(version_incompatible)
            message(FATAL_ERROR "Protobuf ${Protobuf_VERSION} not supported : only 3.0.0 or highter!")
        else(version_incompatible)
            message(STATUS "Found Protobuf ${Protobuf_VERSION} --${Protobuf_LIBRARIES}")
            include_directories(${Protobuf_INCLUDE_DIRS})
            link_directories(${Protobuf_LIBRARY_DIRS})
            #set(REQUIRED_LIBS ${REQUIRED_LIBS} ${Protobuf_LIBRARIES})
        endif(version_incompatible)
    else(Protobuf_FOUND)
        message(STATUS "Protobuf NOT found!")
    endif(Protobuf_FOUND)

endif()
