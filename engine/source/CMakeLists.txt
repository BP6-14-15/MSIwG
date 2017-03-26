CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0 FATAL_ERROR)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

IF (NOT $ENV{CXX} STREQUAL "")
    SET(CMAKE_CXX_COMPILER $ENV{CXX})
ENDIF ()

INCLUDE(CheckCXXCompilerFlag)

PROJECT(GameEngine CXX)

INCLUDE_DIRECTORIES("src")
SUBDIRLIST(SUBDIRS "${CMAKE_SOURCE_DIR}/src")
FOREACH(subdir ${SUBDIRS})
    INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/${subdir}")
ENDFOREACH()

SET(LINKER_FLAGS "-lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf -ldl -lpthread") 

SET(LIBCXXPATH "$ENV{HOME}/Downloads" CACHE STRING "path to installed libc++ using the installation script")

CHECK_CXX_COMPILER_FLAG("-std=c++1z" COMPILER_SUPPORTS_CXX17)

IF(NOT COMPILER_SUPPORTS_CXX17)
    MESSAGE(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has not C++1z support")
ENDIF()


FILE(GLOB_RECURSE SRCS src/*.cpp)
ADD_EXECUTABLE(GameEngine ${SRCS})

LINK_DIRECTORIES("${LIBCXXPATH}/LLVM/build/lib" )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z  -I${LIBCXXPATH}/LLVM/build/include/c++/v1 -nostdinc++")

SET(LIBCXX_FLAGS "-stdlib=libc++ -L${LIBCXXPATH}/LLVM/build/lib -lc++abi -lc++ -nostdinc++")
SET(CMAKE_EXE_LINKER_FLAGS "${LIBCXX_FLAGS} ${LINKER_FLAGS}")

