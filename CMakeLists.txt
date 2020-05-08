#
# @Author: *
# @License: *
# @Date: *
# @Version: 1
# @Description: Makefile
#

cmake_minimum_required(VERSION 3.0)

###############################################################################
# Load modules cmake
###############################################################################
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(auto-install)   # auto install package
include(cppcheck)       # 
include(clang-format)   #
#include(doxygen)       # Create le doc

###############################################################################
# Load modules of the projet
###############################################################################
project(aposd)
set(MODULE_MPI false)
set(MODULE_SAAS true)
set(MODULE_SEQ true)

###############################################################################
# Load modules of external libs
###############################################################################
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

add_subdirectory("modules")


###############################################################################
# Get info git and genereted version.h from version.h.in 
###############################################################################
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
else(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(GIT_BRANCH "")
  set(GIT_COMMIT_HASH "")
endif(EXISTS "${CMAKE_SOURCE_DIR}/.git")

message(STATUS "Git current branch: ${GIT_BRANCH}")
message(STATUS "Git commit hash: ${GIT_COMMIT_HASH}")
configure_file(
  ${CMAKE_SOURCE_DIR}/version.h.in
  ${CMAKE_BINARY_DIR}/generated/version.h
)

include_directories(${CMAKE_BINARY_DIR}/generated)

###############################################################################
# Set of parameters
###############################################################################
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-std=c++2a")
# -pedantic -Wall -Wextra -Wcast-align -Wctor-dtor-privacy    \
#                                 -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op \
#                                 -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept    \
#                                 -Woverloaded-virtual -Wredundant-decls  -Wsign-promo        \
#                                 -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default \
#                                 -Wundef -Werror -Wno-unused") # -Wsign-conversion -Wshadow  -Wcast-qual -Wold-style-cast
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# include(ExternalProject)
# ExternalProject_Add(CPPCMS
#   GIT_REPOSITORY    git@github.com:artyom-beilis/cppcms.git
#   GIT_TAG           origin/release/1.2.1
# )


## PkgConfig
find_package(PkgConfig REQUIRED)
if (NOT PKG_CONFIG_FOUND)
    packageManager("pkg-config")
    find_package(PkgConfig REQUIRED)
endif(NOT PKG_CONFIG_FOUND)

## jsoncpp
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})
if (NOT JSONCPP_FOUND)
    packageManager("libjsoncpp-dev")
    packageManager("libjsoncpp1")
endif(NOT JSONCPP_FOUND)

# find_package(jsoncpp REQUIRED)
# include_directories(${jsoncpp_INCLUDE_DIR})


## MPI
if(${MODULE_MPI})
    find_package(MPI REQUIRED)
#     if (NOT MPI_FOUND)
#     packageManager("mpi-default-dev")
#     endif(NOT MPI_FOUND)
    include_directories(${MPI_INCLUDE_PATH})
endif(${MODULE_MPI})

## Boost - program_options + log
find_package(Boost 1.40 COMPONENTS program_options log REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
if (NOT Boost_FOUND )
    packageManager("libboost-all-dev")
    find_package(Boost 1.40 COMPONENTS program_options log REQUIRED)
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
if(${MODULE_MPI})
    target_link_libraries(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${MPI_LIBRARIES})
endif(${MODULE_MPI})

target_link_libraries(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${Boost_LIBRARIES})

if(${MODULE_SAAS})
    target_link_libraries(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${LIB_CPPCMS_LIBRARIES}) 
    target_link_libraries(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} LINK_PUBLIC /usr/local/lib/libcppcms.so.1.2.1)
endif(${MODULE_SAAS})

target_link_libraries(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${JSONCPP_LIBRARIES})


target_compile_definitions(${PROJECT_NAME}-${CMAKE_BUILD_TYPE} PRIVATE MODULE_MPI=${MODULE_MPI} MODULE_SAAS=${MODULE_SAAS} MODULE_SEQ=${MODULE_SEQ})


## Cppunit
pkg_check_modules(CPPUNIT cppunit)
link_libraries(${CPPUNIT_LIBRARIES})

# ## Aposd unit test
add_executable(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} tests-unitaires/unitTest.cpp)
target_include_directories(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} PRIVATE "modules")
target_include_directories(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} PUBLIC sources/)
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${MPI_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${LIB_CPPCMS_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${JSONCPP_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC /usr/local/lib/libcppcms.so.1.2.1)
target_link_libraries(${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${CPPUNIT_LIBRARIES})

# ## Test unitaire
enable_testing()
add_test(NAME ${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE} COMMAND "${PROJECT_NAME}-unittest-${CMAKE_BUILD_TYPE}")

## Detection du programme python3.7
FIND_PROGRAM(PYTHON3.7_EXECUTABLE python3.7)
if (NOT PYTHON3.7_EXECUTABLE)
    packageManager("python3.7")
    FIND_PROGRAM(PYTHON3.7_EXECUTABLE python3.7)
endif(NOT PYTHON3.7_EXECUTABLE)

set(LIST_OF_FILES 
aposd_saas_collective
aposd_saas_individual
)
foreach(FILE  ${LIST_OF_FILES})
    add_test(NAME ${FILE} COMMAND ${PYTHON3.7_EXECUTABLE} ./tests-aposd/${FILE}.py WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endforeach()


# add_custom_target(run
#     COMMAND ${PROJECT_NAME}-${CMAKE_BUILD_TYPE}
#     DEPENDS ${PROJECT_NAME}-${CMAKE_BUILD_TYPE}
#     WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
# )
