project(Cliff)

cmake_minimum_required(VERSION 2.8)

#########################################
#		         Compiler               #
#########################################

add_definitions(-std=c++11)
add_definitions(-g)

include_directories(include)

file(GLOB_RECURSE compiler_include include/cliff/*)
file(GLOB_RECURSE compiler_source src/*)

add_executable(Cliff ${compiler_include} ${compiler_source} src/executable/main.cpp)
