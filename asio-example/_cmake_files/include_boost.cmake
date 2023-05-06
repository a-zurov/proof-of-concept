
get_property(boost_def GLOBAL PROPERTY BOOST_DEFINED)

if(NOT boost_def STREQUAL "YES")
    set_property(GLOBAL PROPERTY BOOST_DEFINED "YES")

    set(Boost_ADDITIONAL_VERSIONS "1.55.0" "1.56.0" "1.57.0" "1.64.0" "1.65.0" "1.65.1")
    set(Boost_USE_STATIC_LIBS       OFF)
    set(Boost_USE_MULTITHREADED     ON)
    set(Boost_USE_STATIC_RUNTIME    OFF)

    message(STATUS "Boost_USE_STATIC_LIBS: " ${Boost_USE_STATIC_LIBS})

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(Boost_DEBUG FALSE)
    endif()

    find_package(Boost REQUIRED)
    find_package(Boost COMPONENTS thread asio)
    #find_package(Boost COMPONENTS thread atomic system filesystem signals program_options asio)

    if(Boost_FOUND)
        #compare version
        string(COMPARE LESS "${Boost_VERSION}" 1.55.0 version_incompatible)
        if(version_incompatible)
            message(FATAL_ERROR "Boost ${Boost_VERSION} not supported : only 1.55.0 or highter!")
        else(version_incompatible)
            message(STATUS "Found Boost ${Boost_VERSION} --${Boost_LIBRARIES}")
            include_directories(${Boost_INCLUDE_DIRS})
            link_directories(${Boost_LIBRARY_DIRS})
            #set(REQUIRED_LIBS ${REQUIRED_LIBS} ${Boost_LIBRARIES})
        endif(version_incompatible)
    else(Boost_FOUND)
        message(STATUS "Boost NOT found!")
    endif(Boost_FOUND)

endif()

set(REQUIRED_LIBS ${REQUIRED_LIBS} rt dl)
