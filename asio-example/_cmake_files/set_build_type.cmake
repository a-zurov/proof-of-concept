
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
    #set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_SUFFIX "d")
    set(BUILD_MODIFIER "debug")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BUILD_SUFFIX "")
    set(BUILD_MODIFIER "release")
endif()
