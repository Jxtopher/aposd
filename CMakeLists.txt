#
# @Author: *
# @License: *
# @Date: *
# @Version: 1
# @Description: Makefile
#

cmake_minimum_required(VERSION 3.0)

project(aposd)
set(MODULE_MPI true)
set(MODULE_SAAS true)
set(MODULE_SEQ true)


set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-std=c++17 -pedantic -Wall -Wextra -Wcast-align -Wctor-dtor-privacy    \
                                -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op \
                                -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept    \
                                -Woverloaded-virtual -Wredundant-decls  -Wsign-promo        \
                                -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default \
                                -Wundef -Werror -Wno-unused") # -Wsign-conversion -Wshadow  -Wcast-qual -Wold-style-cast
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


## PkgConfig
find_package(PkgConfig REQUIRED)

## jsoncpp
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

find_package(jsoncpp REQUIRED)
include_directories(${jsoncpp_INCLUDE_DIR})


## MPI
if(${MODULE_MPI})
    find_package(MPI REQUIRED)
    include_directories(${MPI_INCLUDE_PATH})
endif(${MODULE_MPI})

## Boost - program_options
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
if (NOT Boost_FOUND )
    message(FATAL_ERROR "apt-get install libboost-all-dev")
endif(NOT Boost_FOUND)

## Cppcms
if(${MODULE_SAAS})
    #pkg_check_modules(CPPCMS cppcms)
    #link_libraries(${CPPCMS_LIBRARIES})
    # find_library(LIB_CPPCMS cppcms)
    include_directories(/usr/local/include)
endif(${MODULE_SAAS})


## Aposd
add_executable(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} sources/main.cpp)
target_link_libraries(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${MPI_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${LIB_CPPCMS_LIBRARIES}) 
target_link_libraries(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${JSONCPP_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} LINK_PUBLIC /usr/local/lib/libcppcms.so.1.2.1)

target_compile_definitions(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} PRIVATE MODULE_MPI=${MODULE_MPI} MODULE_SAAS=${MODULE_SAAS} MODULE_SEQ=${MODULE_SEQ})


## Cppunit
pkg_check_modules(CPPUNIT cppunit)
link_libraries(${CPPUNIT_LIBRARIES})

## Aposd unit test
add_executable(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} sources/unitTest.cpp)
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${MPI_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${LIB_CPPCMS_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${JSONCPP_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC /usr/local/lib/libcppcms.so.1.2.1)
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${CPPUNIT_LIBRARIES})

## Test unitaire
enable_testing()
add_test(NAME ${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} COMMAND "${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE}")


# add_custom_target(run
#     COMMAND ${PROJECT_NAME}-${CMAKE_BUILD_TYPE}
#     DEPENDS ${PROJECT_NAME}-${CMAKE_BUILD_TYPE}
#     WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
# )


## > Doxygen ##################################################################
find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(Doxyfile.in Doxyfile)

    set(DOXYGEN_INPUT ${CMAKE_BINARY_DIR}/Doxyfile)
    set(DOXYGEN_OUTPUT ${APIDOC_DIR}/html/index.html)

    add_custom_target(doc ALL
    COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT} > /dev/null
    COMMAND ${CMAKE_COMMAND} -E echo "Done."
    )

    install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/html 
        DESTINATION share/doc/${CMAKE_PROJECT_NAME})
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
## < Doxygen

## > cppcheck ##################################################################
SET(PROJECT_TRDPARTY_DIR third-party)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
    string(FIND ${SOURCE_FILE} ${PROJECT_TRDPARTY_DIR} PROJECT_TRDPARTY_DIR_FOUND)
    if (NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
        list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
    endif ()
endforeach ()

add_custom_target(
        cppcheck
        COMMAND /usr/bin/cppcheck
        --enable=warning,performance,portability,information,missingInclude
        --std=c++11
        --library=qt.cfg
        --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
        --verbose
        --quiet
        -DMODULE_SAAS -DMODULE_MPI -DMODULE_SEQ
        ${ALL_SOURCE_FILES}
)
## < cppcheck


## > clang-format ##################################################################
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
    string(FIND ${SOURCE_FILE} ${PROJECT_TRDPARTY_DIR} PROJECT_TRDPARTY_DIR_FOUND)
    if (NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
        list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
    endif ()
endforeach ()

add_custom_target(
        clangformat
        COMMAND /usr/bin/clang-format
        -style=file
        -i
        ${ALL_SOURCE_FILES}
)
## < clang-format