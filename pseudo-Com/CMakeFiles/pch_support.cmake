# Try to find precompiled headers support for GCC 4.x and 5.x
# Once done this will define:
#
# Variable:
#   PCHSupport_FOUND
#   PCHSupport_ENABLED
#
# Macro:
#   ADD_PRECOMPILED_HEADER  _targetName _input  _dowarn
#   ADD_PRECOMPILED_HEADER_TO_TARGET _targetName _input _pch_output_to_use _dowarn
#
# Since this macro overides COMPILER_FLAGS on a target, you must use the following
# variables instead.
# set PCH_ADDITIONAL_COMPILER_FLAGS to add extra COMPILER_FLAGS to targets
# set PCH_ADDITIONAL_COMPILER_FLAGS_${targetName} to add extra COMPILER_FLAGS to a specific target
#

IF(CMAKE_COMPILER_IS_GNUCXX)
    EXEC_PROGRAM(${CMAKE_CXX_COMPILER} ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion OUTPUT_VARIABLE gcc_compiler_version)

    IF(gcc_compiler_version MATCHES "4\\.[0-9](\\.[0-9])?")
        SET(PCHSupport_FOUND TRUE)
    ELSE(gcc_compiler_version MATCHES "4\\.[0-9](\\.[0-9])?")
        IF(gcc_compiler_version MATCHES "3\\.4(\\.[0-9])?")
            SET(PCHSupport_FOUND TRUE)
        ENDIF(gcc_compiler_version MATCHES "3\\.4(\\.[0-9])?")
    ENDIF(gcc_compiler_version MATCHES "4\\.[0-9](\\.[0-9])?")
    SET(_PCH_include_prefix "-I")
ELSE(CMAKE_COMPILER_IS_GNUCXX)
    IF( (WIN32 OR WIN64) )
        SET(PCHSupport_FOUND FALSE)
    ELSE( (WIN32 OR WIN64) )
        SET(PCHSupport_FOUND FALSE)
    ENDIF( (WIN32 OR WIN64) )
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(DEFINED PCHSupport_ENABLED AND NOT PCHSupport_ENABLED)
  SET(PCHSupport_FOUND FALSE)
ENDIF(DEFINED PCHSupport_ENABLED AND NOT PCHSupport_ENABLED)

MACRO(_PCH_GET_COMPILE_FLAGS _out_compile_flags)

    STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
    SET(${_out_compile_flags} ${${_flags_var_name}} )

    IF(CMAKE_COMPILER_IS_GNUCXX)
        GET_TARGET_PROPERTY(_targetType ${_PCH_current_target} TYPE)
        IF(${_targetType} STREQUAL SHARED_LIBRARY)
            LIST(APPEND ${_out_compile_flags} "${${_out_compile_flags}} -fPIC")
        ENDIF(${_targetType} STREQUAL SHARED_LIBRARY)
    ELSE(CMAKE_COMPILER_IS_GNUCXX)
        ## TODO ... ? or does it work out of the box
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)

    GET_DIRECTORY_PROPERTY(DIRINC INCLUDE_DIRECTORIES)
    FOREACH(item ${DIRINC})
        LIST(APPEND ${_out_compile_flags} "${_PCH_include_prefix}${item}")
    ENDFOREACH(item)

    LIST(APPEND ${_out_compile_flags} "${_PCH_include_prefix}${CMAKE_CURRENT_SOURCE_DIR}")

    GET_DIRECTORY_PROPERTY(_directory_flags COMPILE_DEFINITIONS)
    LIST(APPEND ${_out_compile_flags} ${_directory_flags})
    LIST(APPEND ${_out_compile_flags} ${CMAKE_CXX_FLAGS})

    SEPARATE_ARGUMENTS(${_out_compile_flags})

ENDMACRO(_PCH_GET_COMPILE_FLAGS)

MACRO(_PCH_GET_COMPILE_COMMAND out_command _input _output)

    FILE(TO_NATIVE_PATH ${_input} _native_input)
    FILE(TO_NATIVE_PATH ${_output} _native_output)

    IF(CMAKE_COMPILER_IS_GNUCXX)
        IF(CMAKE_CXX_COMPILER_ARG1)
            STRING(REGEX REPLACE "^ +" "" pchsupport_compiler_cxx_arg1 ${CMAKE_CXX_COMPILER_ARG1})
            SET(${out_command} ${CMAKE_CXX_COMPILER} ${pchsupport_compiler_cxx_arg1} ${_compile_FLAGS} -x c++-header -o ${_output} ${_input})
        ELSE(CMAKE_CXX_COMPILER_ARG1)
            SET(${out_command} ${CMAKE_CXX_COMPILER} ${_compile_FLAGS} -x c++-header -o ${_output} ${_input})
        ENDIF(CMAKE_CXX_COMPILER_ARG1)
    ELSE(CMAKE_COMPILER_IS_GNUCXX)
        SET(_dummy_str "#include <${_input}>")
        FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/pch_dummy.cpp ${_dummy_str})
        SET(${out_command} ${CMAKE_CXX_COMPILER} ${_compile_FLAGS} /c /Fp${_native_output} /Yc${_native_input} pch_dummy.cpp)
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ENDMACRO(_PCH_GET_COMPILE_COMMAND )

MACRO(_PCH_GET_TARGET_COMPILE_FLAGS _targetName _cflags _header_name _pch_path _dowarn)

    FILE(TO_NATIVE_PATH ${_pch_path} _native_pch_path)

    IF(CMAKE_COMPILER_IS_GNUCXX)
        # for use with distcc and gcc >4.0.1 if preprocessed files are accessible
        # on all remote machines set
        # PCH_ADDITIONAL_COMPILER_FLAGS to -fpch-preprocess
        # if you want warnings for invalid header files (which is very inconvenient
        # if you have different versions of the headers for different build types
        # you may set _pch_dowarn
        IF(_dowarn)
            SET(${_cflags} "${PCH_ADDITIONAL_COMPILER_FLAGS} ${PCH_ADDITIONAL_COMPILER_FLAGS_${_targetName}} -include ${CMAKE_CURRENT_BINARY_DIR}/${_header_name} -Winvalid-pch ")
        ELSE(_dowarn)
            SET(${_cflags} "${PCH_ADDITIONAL_COMPILER_FLAGS} ${PCH_ADDITIONAL_COMPILER_FLAGS_${_targetName}} -include ${CMAKE_CURRENT_BINARY_DIR}/${_header_name} ")
        ENDIF(_dowarn)
    ELSE(CMAKE_COMPILER_IS_GNUCXX)
        set(${_cflags} "/Fp${_native_pch_path} /Yu${_header_name}")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ENDMACRO(_PCH_GET_TARGET_COMPILE_FLAGS )

MACRO(GET_PRECOMPILED_HEADER_OUTPUT _targetName _input _output)

    GET_FILENAME_COMPONENT(_name ${_input} NAME)
    GET_FILENAME_COMPONENT(_path ${_input} PATH)
    SET(_output "${CMAKE_CURRENT_BINARY_DIR}/${_name}.gch/${_targetName}_${CMAKE_BUILD_TYPE}.h++")

ENDMACRO(GET_PRECOMPILED_HEADER_OUTPUT _targetName _input)

MACRO(ADD_PRECOMPILED_HEADER_TO_TARGET _targetName _input _pch_output_to_use )

    IF(PCHSupport_FOUND)
        GET_FILENAME_COMPONENT(_name ${_input} NAME)

        IF( "${ARGN}" STREQUAL "0")
            SET(_dowarn 0)
        ELSE( "${ARGN}" STREQUAL "0")
            SET(_dowarn 1)
        ENDIF("${ARGN}" STREQUAL "0")

        _PCH_GET_TARGET_COMPILE_FLAGS(${_targetName} _target_cflags ${_name} ${_pch_output_to_use} ${_dowarn})
        SET_TARGET_PROPERTIES(${_targetName} PROPERTIES COMPILE_FLAGS ${_target_cflags})
        ADD_CUSTOM_TARGET(pch_Generate_${_targetName} DEPENDS ${_pch_output_to_use})
        ADD_DEPENDENCIES(${_targetName} pch_Generate_${_targetName} )
    ELSE( PCHSupport_FOUND )
        SET_TARGET_PROPERTIES(${_targetName} PROPERTIES COMPILE_FLAGS ${PCH_ADDITIONAL_COMPILER_FLAGS} ${PCH_ADDITIONAL_COMPILER_FLAGS_${_targetName}})
    ENDIF( PCHSupport_FOUND )

ENDMACRO(ADD_PRECOMPILED_HEADER_TO_TARGET)

MACRO(ADD_PRECOMPILED_HEADER _targetName _input)

    IF(PCHSupport_FOUND)
        SET(_PCH_current_target ${_targetName})

        IF(NOT CMAKE_BUILD_TYPE)
            MESSAGE(FATAL_ERROR "This is the ADD_PRECOMPILED_HEADER macro. " "You must set CMAKE_BUILD_TYPE!")
        ENDIF(NOT CMAKE_BUILD_TYPE)

        IF( "${ARGN}" STREQUAL "0")
            SET(_dowarn 0)
        ELSE( "${ARGN}" STREQUAL "0")
            SET(_dowarn 1)
        ENDIF("${ARGN}" STREQUAL "0")

        GET_FILENAME_COMPONENT(_name ${_input} NAME)
        GET_FILENAME_COMPONENT(_path ${_input} PATH)
        GET_FILENAME_COMPONENT(_name_we ${_input} NAME_WE)
        GET_PRECOMPILED_HEADER_OUTPUT( ${_targetName} ${_input} _output)
        GET_FILENAME_COMPONENT(_outdir ${_output} PATH )

        GET_TARGET_PROPERTY(_targetType ${_PCH_current_target} TYPE)
        SET(_pch_cpp ${_path}/${_name_we}.cpp)

        IF(${_targetType} STREQUAL SHARED_LIBRARY)
            ADD_LIBRARY(${_targetName}_pch SHARED ${_pch_cpp})
        ELSE(${_targetType} STREQUAL SHARED_LIBRARY)
            ADD_LIBRARY(${_targetName}_pch STATIC ${_pch_cpp})
        ENDIF(${_targetType} STREQUAL SHARED_LIBRARY)

        FILE(MAKE_DIRECTORY ${_outdir})

        SET(_compile_FLAGS "")
        _PCH_GET_COMPILE_FLAGS(_compile_FLAGS)
        SET(_compile_FLAGS ${_compile_FLAGS} ${PCH_ADDITIONAL_COMPILER_FLAGS} ${PCH_ADDITIONAL_COMPILER_FLAGS_${_targetName}})

        SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_BINARY_DIR}/${_name} PROPERTIES GENERATED 1)

        ADD_CUSTOM_COMMAND(
                OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${_name}
                COMMAND ${CMAKE_COMMAND} -E copy  ${_input} ${CMAKE_CURRENT_BINARY_DIR}/${_name}
                DEPENDS ${_input}
            )

        _PCH_GET_COMPILE_COMMAND(_command  ${CMAKE_CURRENT_BINARY_DIR}/${_name} ${_output})

        ADD_CUSTOM_COMMAND(
                OUTPUT ${_output}
                COMMAND ${_command}
                DEPENDS ${_input}   ${CMAKE_CURRENT_BINARY_DIR}/${_name} ${_targetName}_pch
            )

        ADD_PRECOMPILED_HEADER_TO_TARGET(${_targetName} ${_input}  ${_output} ${_dowarn})
    ENDIF(PCHSupport_FOUND)

ENDMACRO(ADD_PRECOMPILED_HEADER)

