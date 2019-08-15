cmake_minimum_required(VERSION 3.0)
project(aposd)


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

## jsoncpp
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

## MPI
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

## Boost - program_options
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

## Cppcms
#pkg_check_modules(CPPCMS cppcms)
#link_libraries(${CPPCMS_LIBRARIES})
# find_library(LIB_CPPCMS cppcms)
include_directories(/usr/local/include)

## Doxygen
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


##
add_executable(aposd-${CMAKE_BUILD_TYPE} sources/main.cpp)
target_link_libraries(aposd-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${MPI_LIBRARIES})
target_link_libraries(aposd-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(aposd-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${LIB_CPPCMS_LIBRARIES})
target_link_libraries(aposd-${CMAKE_BUILD_TYPE} LINK_PUBLIC ${JSONCPP_LIBRARIES})
target_link_libraries(aposd-${CMAKE_BUILD_TYPE} LINK_PUBLIC /usr/local/lib/libcppcms.so.1.2.1)