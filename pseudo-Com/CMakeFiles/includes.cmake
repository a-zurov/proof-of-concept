
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
    #set(CMAKE_BUILD_TYPE Release)
endif()

set(BUILD_MODIFIER "release")
set(BUILD_SUFFIX "")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_SUFFIX "d")
    set(BUILD_MODIFIER "debug")
endif()

get_property(BOOST_D GLOBAL PROPERTY BOOST_DEFINED)

if(NOT BOOST_D STREQUAL "YES")
    set_property(GLOBAL PROPERTY BOOST_DEFINED "YES")

    #Find Boost

    set(Boost_ADDITIONAL_VERSIONS "1.49" "1.49.0" "1.50" "1.50.0" "1.51" "1.51.0" "1.52" "1.52.0" "1.53" "1.53.0" "1.54" "1.54.0" "1.55" "1.55.0" "1.56.0" "1.57.0" "1.64.0" "1.65.1")
    set(Boost_USE_STATIC_LIBS       OFF)
    set(Boost_USE_MULTITHREADED     ON)
    set(Boost_USE_STATIC_RUNTIME    OFF)

    message(STATUS "Boost_USE_STATIC_LIBS: " ${Boost_USE_STATIC_LIBS})

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(Boost_DEBUG FALSE)
    endif()

    #set(Boost_NO_BOOST_CMAKE ON)

    find_package(Boost 1.65.1 REQUIRED)
    find_package(Boost COMPONENTS thread atomic system filesystem signals program_options)

    if(Boost_FOUND)
        #compare version strings
        string(COMPARE LESS "${Boost_VERSION}" 1.41.0 VersionIncompatible)
        if(VersionIncompatible)
            message(FATAL_ERROR "Boost ${Boost_VERSION} not supported. Only 1.41.0 or highter version is supported.")
        else(VersionIncompatible)
            message(STATUS "Found Boost ${Boost_VERSION} --${Boost_LIBRARIES}")
            include_directories(${Boost_INCLUDE_DIRS})
            link_directories(${Boost_LIBRARY_DIRS})
            set(REQUIRED_LIBS ${REQUIRED_LIBS} ${Boost_LIBRARIES})
        endif(VersionIncompatible)
    else(Boost_FOUND)
        message(STATUS "Boost NOT FOUND")
    endif(Boost_FOUND)

    #end of Find Boost

endif()

set(REQUIRED_LIBS ${REQUIRED_LIBS} rt dl)

