cmake_minimum_required(VERSION 3.21)

project(
	Split
    LANGUAGES CXX
)


file (GLOB_RECURSE  Src  *.cpp  *.h )

 set (exec Split)
add_executable(${exec}  ${Src} )

catch_discover_tests(${exec})