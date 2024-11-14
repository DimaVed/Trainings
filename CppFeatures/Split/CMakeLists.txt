cmake_minimum_required(VERSION 3.21)

project(
	Split
    LANGUAGES CXX
)


file (GLOB_RECURSE  Src  *.cpp  *.h )
add_executable(Split  ${Src} )

