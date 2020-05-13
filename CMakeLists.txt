#
# @Author: *
# @License: *
# @Date: *
# @Version: 1
# @Description: CMakeLists
#

cmake_minimum_required(VERSION 3.0)

###############################################################################
# Set of parameters
###############################################################################
set(CMAKE_BUILD_TYPE Release) # (Release|Debug)

set(CMAKE_CXX_FLAGS "-std=c++2a")
# -pedantic -Wall -Wextra -Wcast-align -Wctor-dtor-privacy    \
#                                 -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op \
#                                 -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept    \
#                                 -Woverloaded-virtual -Wredundant-decls  -Wsign-promo        \
#                                 -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default \
#                                 -Wundef -Werror -Wno-unused") # -Wsign-conversion -Wshadow  -Wcast-qual -Wold-style-cast
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

###############################################################################
# Load modules cmake
###############################################################################
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(auto-install)   # auto install package
include(cppcheck)       # Check code
include(clang-format)   # Code formatting
include(version)        # Version name generator form git branch, hash
include(doxygen)        # Create le doc

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
add_executable(${PROJECT_NAME} sources/main.cpp)
# file(GLOB_RECURSE ALL_SOURCE_FILES  sources/*.hpp)
# target_precompile_headers(${PROJECT_NAME} PRIVATE ${ALL_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} LINK_PRIVATE 
    ${Boost_LIBRARIES}
    ${JSONCPP_LIBRARIES}
)

if(${MODULE_MPI})
    target_link_libraries(${PROJECT_NAME} LINK_PRIVATE ${MPI_LIBRARIES})
endif(${MODULE_MPI})

if(${MODULE_SAAS})
    target_link_libraries(${PROJECT_NAME} LINK_PRIVATE ${LIB_CPPCMS_LIBRARIES}) 
    target_link_libraries(${PROJECT_NAME} LINK_PRIVATE /usr/local/lib/libcppcms.so.1.2.1)
endif(${MODULE_SAAS})

target_compile_definitions(${PROJECT_NAME} 
    PRIVATE MODULE_MPI=${MODULE_MPI} 
    MODULE_SAAS=${MODULE_SAAS} 
    MODULE_SEQ=${MODULE_SEQ}
    BOOST_LOG_DYN_LINK=1)

## Cppunit
pkg_check_modules(CPPUNIT cppunit)
link_libraries(${CPPUNIT_LIBRARIES})

# ## Aposd unit test
add_executable(${PROJECT_NAME}-unittest tests-unitaires/unitTest.cpp)
target_include_directories(${PROJECT_NAME}-unittest PRIVATE "modules")
target_include_directories(${PROJECT_NAME}-unittest PRIVATE sources/)
target_link_libraries(${PROJECT_NAME}-unittest LINK_PRIVATE 
    ${Boost_LIBRARIES}
    ${MPI_LIBRARIES}
    ${LIB_CPPCMS_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    /usr/local/lib/libcppcms.so.1.2.1
    ${CPPUNIT_LIBRARIES}
)

## Test unitaire
enable_testing()
add_test(NAME ${PROJECT_NAME}-unittest COMMAND "${PROJECT_NAME}-unittest")

## Detection du programme python3.7
FIND_PROGRAM(PYTHON3.7_EXECUTABLE python3.7)
if (NOT PYTHON3.7_EXECUTABLE)
    packageManager("python3.7")
    FIND_PROGRAM(PYTHON3.7_EXECUTABLE python3.7)
endif(NOT PYTHON3.7_EXECUTABLE)

file(GLOB LIST_OF_FILES  tests-aposd/*.py)
foreach(PATH_FILE  ${LIST_OF_FILES})
    get_filename_component(FILE ${PATH_FILE} NAME)
    add_test(NAME ${FILE} COMMAND ${PYTHON3.7_EXECUTABLE} ${PATH_FILE} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endforeach()