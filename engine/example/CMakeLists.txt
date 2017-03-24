cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
include(CheckCXXCompilerFlag)

IF (NOT $ENV{CXX} STREQUAL "")
    SET(CMAKE_CXX_COMPILER $ENV{CXX})
ENDIF ()

project(GameClient CXX)

CHECK_CXX_COMPILER_FLAG("-std=c++1z" COMPILER_SUPPORTS_CXX17)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(LIBCXXPATH "$ENV{HOME}/Downloads" CACHE STRING "path to installed libc++ using the installation script")

if(COMPILER_SUPPORTS_CXX17)
    set(LIBCXX_FLAGS "-stdlib=libc++  -L${LIBCXXPATH}/LLVM/build/lib")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z  -I${LIBCXXPATH}/LLVM/build/include/c++/v1 -nostdinc++")
    set(CMAKE_LIBRARY_LINKER_FLAG "${CMAKE_LIBRARY_LINKER_FLAGS} ${LIBCXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAG "${CMAKE_EXE_LINKER_FLAGS} ${LIBCXX_FLAGS}")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} does not support C++1z")
    
endif()


file(GLOB_RECURSE SRCS src/*.cpp)
file(GLOB_RECURSE HDRS src/*.hpp)

ADD_LIBRARY(clientA SHARED ${SRCS} ${HDRS})

