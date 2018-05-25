project(Cliff)

cmake_minimum_required(VERSION 2.8)

#########################################
#		         Compiler               #
#########################################

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)
if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX1Y)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()

set(APPS_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ansi -pedantic -Wshadow -Weffc++ -ftemplate-backtrace-limit=0  -msse -msse2 -msse3 -march=native -mfpmath=sse -mstackrealign")

include_directories(include)

file(GLOB_RECURSE compiler_include include/cliff/*)
file(GLOB_RECURSE compiler_source src/*)

add_executable(Cliff ${compiler_include} ${compiler_source} src/executable/main.cpp)
