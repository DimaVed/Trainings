cmake_minimum_required(VERSION 3.21)

project(
	Range
    LANGUAGES CXX
)


file (GLOB_RECURSE  Src  *.cpp  *.h )


set (exec Range)
add_executable(${exec}  ${Src} )
catch_discover_tests(${exec})
