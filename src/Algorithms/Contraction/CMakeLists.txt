cmake_minimum_required(VERSION 3.21)

project(
	Contraction
    LANGUAGES CXX
)

include_directories(${CMAKE_BINARY_DIR})
file (GLOB_RECURSE  Src  *.cpp  *.h )
set (exec Contraction)
add_executable (${exec} ${Src})
catch_discover_tests(${exec}  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
