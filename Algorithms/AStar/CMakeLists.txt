cmake_minimum_required(VERSION 3.21)

project(
	AStar
    LANGUAGES CXX
)


file (GLOB_RECURSE  Src  *.cpp  *.h )
set (exec AStar)
add_executable(${exec}  ${Src} )

